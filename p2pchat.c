#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "socket.h"
#include "ui.h"

// Keep the username in a global so we can access it from the callback
const char* username;

unsigned short port;

typedef struct header {
  int message_length;
  int username_length;
} header;

typedef struct LinkedList{
    unsigned short data;
    struct LinkedList *next;
 } LinkedList;

 typedef struct LinkedList *node;

 node* head = NULL;

node createNode() {
    node temp;
    temp = (node)malloc(sizeof(struct LinkedList));
    temp->next = NULL;
    return temp;
}

node addNode(unsigned short value) {
    node temp,p;
    temp = createNode();
    temp->data = value; 
    if(head == NULL){
        head = temp;     
    }
    else{
        p  = head;
        while(p->next != NULL){
            p = p->next;
        }
        p->next = temp;
    }
    return head;
}

// This function is run whenever the user hits enter after typing a message
void input_callback(const char* message) {
  if(strcmp(message, ":quit") == 0 || strcmp(message, ":q") == 0) {
    ui_exit();
  } else {
    ui_display(username, message);
    pthread_t thread = pthread_create(thread, NULL, message_writer, message);
  }
}

void message_reader(unsigned short port) {
  struct header h = malloc(sizeof(header));
  const char* message;
  const char* peer_user;
  read(port, &h, sizeof(struct header));
  read(port, peer_user, h.username_length);
  if(read(port, message, h.message_length) > 0) {
  	ui_display(peer_user, message);
  }
}

void message_writer(void* message) {
  struct header h = malloc(sizeof(header));
  h.username_length = strlen(username);
  h.message_length = strlen(message);
  node p;
  p = head;
  while(p != NULL) {
  	write(p, &h, sizeof(struct header));
  	write(p, username, strlen(username));
  	write(p, message, strlen(message));
  	p = p->next;
}

int main(int argc, char** argv) {
  // Make sure the arguments include a username
  if(argc != 2 && argc != 4) {
    fprintf(stderr, "Usage: %s <username> [<peer> <port number>]\n", argv[0]);
    exit(1);
  }
  
  // Save the username in a global
  username = argv[1];
  
  // Open server socket
  unsigned short port = 0;
  int server_socket_fd = server_socket_open(&port);
  if (server_socket_fd == -1) {
    perror("Server socket was not opened.\n");
    exit(2);
  }
  // Listen to server socket
  if(listen(server_socket_fd, 1)) {
    perror("Listen failed.\n");
    exit(2);
  }

  addNode(port);
  
  // Did the user specify a peer we should connect to?
  if(argc == 4) {
    // Unpack arguments
    char* peer_hostname = argv[2];
    unsigned short peer_port = atoi(argv[3]);
    
    // Connect peer to server socket
    peer_socket_fd = socket_connect(peer_hostname, peer_port);
    if (peer_socket_fd == -1) {
      perror("Server socket was not opened.\n");
      exit(2);
    }

    //save info to data structure (Is it the fd or the peer port that we want?)
    addNode(peer_port);
  }
  
  // Set up the user interface. The input_callback function will be called
  // each time the user hits enter to send a message.
  ui_init(input_callback);
  
  // Keep reading for messages from all sockets available
  pthread_t threads[thread_count];
  thread_args_t args[thread_count];
for(int i = 0; i < peers.size; i++) {
    args[i].lst = &lst;
    args[i].value_count = value_count;

    if(pthread_create(&threads[i], NULL, message_reader, &args[i])) {
      perror("pthread_create failed");
      exit(2);
    }
  }

  // Once the UI is running, you can use it to display log messages
  ui_display("INFO", "This is a handy log message.");
  
  // Run the UI loop. This function only returns once we call ui_stop() somewhere in the program.
  ui_run();
  
  return 0;
}
