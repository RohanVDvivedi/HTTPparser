#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

extern const int http_status_codes[];
extern const char* http_status_code_strings[];

char* get_http_status_line(int status);

#endif