#ifndef BASE64_H
#define BASE64_H

#include <iostream>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <string>
#include <string.h>

std::string base64_decode(const std::string &input) {
  BIO *bio, *b64;
  char *buffer = (char *)malloc(input.length());
  memset(buffer, 0, input.length());

  bio = BIO_new_mem_buf(input.c_str(), -1);
  b64 = BIO_new(BIO_f_base64());
  bio = BIO_push(b64, bio);

  // Do not use newlines to flush buffer
  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

  size_t length = BIO_read(bio, buffer, input.length());
  BIO_free_all(bio);

  std::string output(buffer, length);
  free(buffer);
  return output;
}

std::string base64_encode(const std::string &input) {
  BIO *bio, *b64;
  BUF_MEM *bufferPtr;

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);

  // Ignore newlines - write everything in one line
  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

  BIO_write(bio, input.c_str(), input.length());
  BIO_flush(bio);
  BIO_get_mem_ptr(bio, &bufferPtr);
  BIO_set_close(bio, BIO_NOCLOSE);
  BIO_free_all(bio);

  std::string output(bufferPtr->data, bufferPtr->length);
  BUF_MEM_free(bufferPtr);
  return output;
}

#endif
