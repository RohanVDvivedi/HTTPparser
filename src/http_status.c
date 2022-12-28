#include<http_status.h>

#include<stddef.h>

const char http_status_lines[57][50] =
{
	"100  Continue\r\n",
	"101  Switching Protocols\r\n",
	"102  Processing\r\n",
	"200  OK\r\n",
	"201  Created\r\n",
	"202  Accepted\r\n",
	"203  Non-Authoritative Information\r\n",
	"204  No Content\r\n",
	"205  Reset Content\r\n",
	"206  Partial Content\r\n",
	"207  Multi-Status\r\n",
	"300  Multiple Choices\r\n",
	"301  Moved Permanently\r\n",
	"302  Found\r\n",
	"303  See Other\r\n",
	"304  Not Modified\r\n",
	"305  Use Proxy\r\n",
	"307  Temporary Redirect\r\n",
	"308  Permanent Redirect\r\n",
	"400  Bad Request\r\n",
	"401  Unauthorized\r\n",
	"402  Payment Required\r\n",
	"403  Forbidden\r\n",
	"404  Not Found\r\n",
	"405  Method Not Allowed\r\n",
	"406  Not Acceptable\r\n",
	"407  Proxy Authentication Required\r\n",
	"408  Request Time-out\r\n",
	"409  Conflict\r\n",
	"410  Gone\r\n",
	"411  Length Required\r\n",
	"412  Precondition Failed\r\n",
	"413  Request Entity Too Large\r\n",
	"414  Request-URI Too Large\r\n",
	"415  Unsupported Media Type\r\n",
	"416  Request Range Not Satisfiable\r\n",
	"417  Expectation Failed\r\n",
	"421  Misdirected Request\r\n",
	"422  Unprocessable Entity\r\n",
	"423  Locked\r\n",
	"424  Failed Dependency\r\n",
	"425  Unordered Collection\r\n",
	"426  Upgrade Required\r\n",
	"428  Precondition Required\r\n",
	"429  Too Many Requests\r\n",
	"431  Request Header Fields Too Large\r\n",
	"451  Unavailable For Legal Reasons\r\n",
	"500  Internal Server Error\r\n",
	"502  Bad Gateway\r\n",
	"503  Service Unavailable\r\n",
	"504  Gateway Time-out\r\n",
	"505  HTTP Version Not Supported\r\n",
	"506  Variant Also Negotiates\r\n",
	"507  Insufficient Storage\r\n",
	"508  Loop Detected\r\n",
	"510  Not Extended\r\n",
	"511  Network Authentication Required\r\n"
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