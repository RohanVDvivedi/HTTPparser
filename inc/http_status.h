#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

extern const int http_status_codes[];
extern const char* http_status_code_strings[];

char* get_http_status_line(int status);

// returns -1 for error and 0 for success
int parse_http_status(stream* rs, int* s);

// returns -1 for error and 0 for success
int serialize_http_status(stream* ws, const int* s);

#endif