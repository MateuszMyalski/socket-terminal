#include "client_session.hpp"

#include <string.h>

#include <vector>

ClientSession::ClientSession(struct sockaddr_in in_address,
                             const int client_socket) {
  this->in_address = in_address;
  this->client_socket = client_socket;
  this->session_alive = false;
  this->repr_ip = inet_ntoa(this->in_address.sin_addr);
}

void ClientSession::start_session() {
  int pt_status = 0;
  pthread_mutex_init(&this->mutex, 0);
  std::cout << "Creating session for: " << this->repr_ip << std::endl;
  pt_status = pthread_create(&this->thread_id, NULL,
                             ClientSession::thread_wrapper, this);
  if (pt_status != 0) {
    perror("pthread_create()");
  }

  // Wait till the sesion fully starts to sync
  while (!this->is_session_alive())
    ;
}

void ClientSession::end_session() {
  int pt_status = 0;
  std::cout << "Finishing session for: " << this->repr_ip
            << std::endl;

  pthread_mutex_destroy(&this->mutex);
  pthread_mutex_lock(&this->mutex);
  if (this->session_alive)
    this->session_alive = false;
  else
    return;
  pthread_mutex_unlock(&this->mutex);
  pt_status = pthread_join(this->thread_id, NULL);
  if (pt_status != 0) {
    perror("pthread_join()");
  }
}

bool ClientSession::is_session_alive() { return this->session_alive; }

void *ClientSession::thread_wrapper(void *context) {
  ClientSession *_context = static_cast<ClientSession *>(context);
  _context->connection_thread();
  pthread_mutex_lock(&_context->mutex);
  _context->session_alive = false;
  pthread_mutex_unlock(&_context->mutex);

  return NULL;
}

void ClientSession::connection_thread() {
  pthread_mutex_lock(&this->mutex);
  this->session_alive = true;
  pthread_mutex_unlock(&this->mutex);

  std::string recv_buffer;

  while (1) {
    std::vector<char> buffer(this->max_packet_size);
    pthread_mutex_lock(&this->mutex);
    if (!this->session_alive) {
      std::cout << "Session forced to close for: " << this->repr_ip
                << std::endl;
      break;
    }
    pthread_mutex_unlock(&this->mutex);

    int n_recv = recv(this->client_socket, &buffer[0], buffer.size(), 0);
    if (n_recv <= 0) {
      perror("recv()");
      break;
    }

    std::cout << "Msg.(" << n_recv << ") from: " << this->repr_ip << ": ";
    for (const auto &symbol : buffer) {
      if (symbol == 0) continue;
      if (symbol == 27) {
        std::cout << std::endl;
        return;
      };
      if (isprint(symbol)) {
        recv_buffer.push_back(symbol);
        std::cout << symbol;
      } else {
        std::cout << "[" << static_cast<int>(symbol) << "]";
      }
    }
    std::cout << std::endl;
  }
}

ClientSession::~ClientSession() {  // neet to add exit in thread flag
  this->end_session();
}
