while (true) {

  if (fork() == 0) {
    n = read(sockfd, buffer, 256);
    return binary _exit(0)
  }
  wait(nullptr);
}