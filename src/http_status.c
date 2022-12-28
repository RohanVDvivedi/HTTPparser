#include<http_status.h>

#include<stddef.h>

const char* http_status_lines[57] =
{
	"100  Continue",
	"101  Switching Protocols",
	"102  Processing",
	"200  OK",
	"201  Created",
	"202  Accepted",
	"203  Non-Authoritative Information",
	"204  No Content",
	"205  Reset Content",
	"206  Partial Content",
	"207  Multi-Status",
	"300  Multiple Choices",
	"301  Moved Permanently",
	"302  Found",
	"303  See Other",
	"304  Not Modified",
	"305  Use Proxy",
	"307  Temporary Redirect",
	"308  Permanent Redirect",
	"400  Bad Request",
	"401  Unauthorized",
	"402  Payment Required",
	"403  Forbidden",
	"404  Not Found",
	"405  Method Not Allowed",
	"406  Not Acceptable",
	"407  Proxy Authentication Required",
	"408  Request Time-out",
	"409  Conflict",
	"410  Gone",
	"411  Length Required",
	"412  Precondition Failed",
	"413  Request Entity Too Large",
	"414  Request-URI Too Large",
	"415  Unsupported Media Type",
	"416  Request Range Not Satisfiable",
	"417  Expectation Failed",
	"421  Misdirected Request",
	"422  Unprocessable Entity",
	"423  Locked",
	"424  Failed Dependency",
	"425  Unordered Collection",
	"426  Upgrade Required",
	"428  Precondition Required",
	"429  Too Many Requests",
	"431  Request Header Fields Too Large",
	"451  Unavailable For Legal Reasons",
	"500  Internal Server Error",
	"502  Bad Gateway",
	"503  Service Unavailable",
	"504  Gateway Time-out",
	"505  HTTP Version Not Supported",
	"506  Variant Also Negotiates",
	"507  Insufficient Storage",
	"508  Loop Detected",
	"510  Not Extended",
	"511  Network Authentication Required"
};

char* get_http_status_line(int status)
{
	switch(status)
	{
		case 100 :
			return (char*) http_status_lines[0];
		case 101 :
			return (char*) http_status_lines[1];
		case 102 :
			return (char*) http_status_lines[2];
		case 200 :
			return (char*) http_status_lines[3];
		case 201 :
			return (char*) http_status_lines[4];
		case 202 :
			return (char*) http_status_lines[5];
		case 203 :
			return (char*) http_status_lines[6];
		case 204 :
			return (char*) http_status_lines[7];
		case 205 :
			return (char*) http_status_lines[8];
		case 206 :
			return (char*) http_status_lines[9];
		case 207 :
			return (char*) http_status_lines[10];
		case 300 :
			return (char*) http_status_lines[11];
		case 301 :
			return (char*) http_status_lines[12];
		case 302 :
			return (char*) http_status_lines[13];
		case 303 :
			return (char*) http_status_lines[14];
		case 304 :
			return (char*) http_status_lines[15];
		case 305 :
			return (char*) http_status_lines[16];
		case 307 :
			return (char*) http_status_lines[17];
		case 308 :
			return (char*) http_status_lines[18];
		case 400 :
			return (char*) http_status_lines[19];
		case 401 :
			return (char*) http_status_lines[20];
		case 402 :
			return (char*) http_status_lines[21];
		case 403 :
			return (char*) http_status_lines[22];
		case 404 :
			return (char*) http_status_lines[23];
		case 405 :
			return (char*) http_status_lines[24];
		case 406 :
			return (char*) http_status_lines[25];
		case 407 :
			return (char*) http_status_lines[26];
		case 408 :
			return (char*) http_status_lines[27];
		case 409 :
			return (char*) http_status_lines[28];
		case 410 :
			return (char*) http_status_lines[29];
		case 411 :
			return (char*) http_status_lines[30];
		case 412 :
			return (char*) http_status_lines[31];
		case 413 :
			return (char*) http_status_lines[32];
		case 414 :
			return (char*) http_status_lines[33];
		case 415 :
			return (char*) http_status_lines[34];
		case 416 :
			return (char*) http_status_lines[35];
		case 417 :
			return (char*) http_status_lines[36];
		case 421 :
			return (char*) http_status_lines[37];
		case 422 :
			return (char*) http_status_lines[38];
		case 423 :
			return (char*) http_status_lines[39];
		case 424 :
			return (char*) http_status_lines[40];
		case 425 :
			return (char*) http_status_lines[41];
		case 426 :
			return (char*) http_status_lines[42];
		case 428 :
			return (char*) http_status_lines[43];
		case 429 :
			return (char*) http_status_lines[44];
		case 431 :
			return (char*) http_status_lines[45];
		case 451 :
			return (char*) http_status_lines[46];
		case 500 :
			return (char*) http_status_lines[47];
		case 502 :
			return (char*) http_status_lines[48];
		case 503 :
			return (char*) http_status_lines[49];
		case 504 :
			return (char*) http_status_lines[50];
		case 505 :
			return (char*) http_status_lines[51];
		case 506 :
			return (char*) http_status_lines[52];
		case 507 :
			return (char*) http_status_lines[53];
		case 508 :
			return (char*) http_status_lines[54];
		case 510 :
			return (char*) http_status_lines[55];
		case 511 :
			return (char*) http_status_lines[56];
		default :
			return NULL;
	}
}