#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

extern const char http_status_lines[57][50];

char* get_http_status_line(int status);

#endif