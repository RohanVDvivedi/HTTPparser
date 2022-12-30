#include<http_status.h>

#include<stddef.h>
#include<string.h>

const int http_status_codes[] =
{
	100,
	101,
	102,
	200,
	201,
	202,
	203,
	204,
	205,
	206,
	207,
	300,
	301,
	302,
	303,
	304,
	305,
	307,
	308,
	400,
	401,
	402,
	403,
	404,
	405,
	406,
	407,
	408,
	409,
	410,
	411,
	412,
	413,
	414,
	415,
	416,
	417,
	421,
	422,
	423,
	424,
	425,
	426,
	428,
	429,
	431,
	451,
	500,
	502,
	503,
	504,
	505,
	506,
	507,
	508,
	510,
	511
};

const char* http_status_code_reason_strings[] =
{
	/*100*/  "Continue",
	/*101*/  "Switching Protocols",
	/*102*/  "Processing",
	/*200*/  "OK",
	/*201*/  "Created",
	/*202*/  "Accepted",
	/*203*/  "Non-Authoritative Information",
	/*204*/  "No Content",
	/*205*/  "Reset Content",
	/*206*/  "Partial Content",
	/*207*/  "Multi-Status",
	/*300*/  "Multiple Choices",
	/*301*/  "Moved Permanently",
	/*302*/  "Found",
	/*303*/  "See Other",
	/*304*/  "Not Modified",
	/*305*/  "Use Proxy",
	/*307*/  "Temporary Redirect",
	/*308*/  "Permanent Redirect",
	/*400*/  "Bad Request",
	/*401*/  "Unauthorized",
	/*402*/  "Payment Required",
	/*403*/  "Forbidden",
	/*404*/  "Not Found",
	/*405*/  "Method Not Allowed",
	/*406*/  "Not Acceptable",
	/*407*/  "Proxy Authentication Required",
	/*408*/  "Request Time-out",
	/*409*/  "Conflict",
	/*410*/  "Gone",
	/*411*/  "Length Required",
	/*412*/  "Precondition Failed",
	/*413*/  "Request Entity Too Large",
	/*414*/  "Request-URI Too Large",
	/*415*/  "Unsupported Media Type",
	/*416*/  "Request Range Not Satisfiable",
	/*417*/  "Expectation Failed",
	/*421*/  "Misdirected Request",
	/*422*/  "Unprocessable Entity",
	/*423*/  "Locked",
	/*424*/  "Failed Dependency",
	/*425*/  "Unordered Collection",
	/*426*/  "Upgrade Required",
	/*428*/  "Precondition Required",
	/*429*/  "Too Many Requests",
	/*431*/  "Request Header Fields Too Large",
	/*451*/  "Unavailable For Legal Reasons",
	/*500*/  "Internal Server Error",
	/*502*/  "Bad Gateway",
	/*503*/  "Service Unavailable",
	/*504*/  "Gateway Time-out",
	/*505*/  "HTTP Version Not Supported",
	/*506*/  "Variant Also Negotiates",
	/*507*/  "Insufficient Storage",
	/*508*/  "Loop Detected",
	/*510*/  "Not Extended",
	/*511*/  "Network Authentication Required"
};

char* get_http_status_line(int status)
{
	switch(status)
	{
		case 100 :
			return (char*) http_status_code_reason_strings[0];
		case 101 :
			return (char*) http_status_code_reason_strings[1];
		case 102 :
			return (char*) http_status_code_reason_strings[2];
		case 200 :
			return (char*) http_status_code_reason_strings[3];
		case 201 :
			return (char*) http_status_code_reason_strings[4];
		case 202 :
			return (char*) http_status_code_reason_strings[5];
		case 203 :
			return (char*) http_status_code_reason_strings[6];
		case 204 :
			return (char*) http_status_code_reason_strings[7];
		case 205 :
			return (char*) http_status_code_reason_strings[8];
		case 206 :
			return (char*) http_status_code_reason_strings[9];
		case 207 :
			return (char*) http_status_code_reason_strings[10];
		case 300 :
			return (char*) http_status_code_reason_strings[11];
		case 301 :
			return (char*) http_status_code_reason_strings[12];
		case 302 :
			return (char*) http_status_code_reason_strings[13];
		case 303 :
			return (char*) http_status_code_reason_strings[14];
		case 304 :
			return (char*) http_status_code_reason_strings[15];
		case 305 :
			return (char*) http_status_code_reason_strings[16];
		case 307 :
			return (char*) http_status_code_reason_strings[17];
		case 308 :
			return (char*) http_status_code_reason_strings[18];
		case 400 :
			return (char*) http_status_code_reason_strings[19];
		case 401 :
			return (char*) http_status_code_reason_strings[20];
		case 402 :
			return (char*) http_status_code_reason_strings[21];
		case 403 :
			return (char*) http_status_code_reason_strings[22];
		case 404 :
			return (char*) http_status_code_reason_strings[23];
		case 405 :
			return (char*) http_status_code_reason_strings[24];
		case 406 :
			return (char*) http_status_code_reason_strings[25];
		case 407 :
			return (char*) http_status_code_reason_strings[26];
		case 408 :
			return (char*) http_status_code_reason_strings[27];
		case 409 :
			return (char*) http_status_code_reason_strings[28];
		case 410 :
			return (char*) http_status_code_reason_strings[29];
		case 411 :
			return (char*) http_status_code_reason_strings[30];
		case 412 :
			return (char*) http_status_code_reason_strings[31];
		case 413 :
			return (char*) http_status_code_reason_strings[32];
		case 414 :
			return (char*) http_status_code_reason_strings[33];
		case 415 :
			return (char*) http_status_code_reason_strings[34];
		case 416 :
			return (char*) http_status_code_reason_strings[35];
		case 417 :
			return (char*) http_status_code_reason_strings[36];
		case 421 :
			return (char*) http_status_code_reason_strings[37];
		case 422 :
			return (char*) http_status_code_reason_strings[38];
		case 423 :
			return (char*) http_status_code_reason_strings[39];
		case 424 :
			return (char*) http_status_code_reason_strings[40];
		case 425 :
			return (char*) http_status_code_reason_strings[41];
		case 426 :
			return (char*) http_status_code_reason_strings[42];
		case 428 :
			return (char*) http_status_code_reason_strings[43];
		case 429 :
			return (char*) http_status_code_reason_strings[44];
		case 431 :
			return (char*) http_status_code_reason_strings[45];
		case 451 :
			return (char*) http_status_code_reason_strings[46];
		case 500 :
			return (char*) http_status_code_reason_strings[47];
		case 502 :
			return (char*) http_status_code_reason_strings[48];
		case 503 :
			return (char*) http_status_code_reason_strings[49];
		case 504 :
			return (char*) http_status_code_reason_strings[50];
		case 505 :
			return (char*) http_status_code_reason_strings[51];
		case 506 :
			return (char*) http_status_code_reason_strings[52];
		case 507 :
			return (char*) http_status_code_reason_strings[53];
		case 508 :
			return (char*) http_status_code_reason_strings[54];
		case 510 :
			return (char*) http_status_code_reason_strings[55];
		case 511 :
			return (char*) http_status_code_reason_strings[56];
		default :
			return NULL;
	}
}

int parse_http_status(stream* rs, int* s)
{
	char byte;
	unsigned int byte_read = 0;
	int error = 0;

	(*s) = 0;

	for(int i = 0; i < 3; i++)
	{
		byte_read = read_from_stream(rs, &byte, 1, &error);
		if(byte_read == 0 || error != 0)
			return -1;

		if(('0' <= byte) && (byte <= '9'))
			(*s) = (*s) * 10 + (byte - '0');
		else
			return -1;
	}

	const char* status_reason_string = get_http_status_line((*s));
	if(status_reason_string == NULL) // this check ensures that it is a valid status code
		return -1;

	int max_spaces = 3;

	unsigned int spaces_seen = 0;
	while(spaces_seen <= max_spaces)
	{
		byte_read = read_from_stream(rs, &byte, 1, &error);
		if(byte_read == 0 || error != 0)
			return -1;

		if(byte == ' ')
			spaces_seen++;
		else
		{
			unread_from_stream(rs, &byte, 1);
			break;
		}
	}

	if(spaces_seen > max_spaces)
		return -1;

	int largest_reason_phrase = 50;

	int last_char_CR = 0;
	unsigned int reason_phrase_bytes_read = 0;
	int status_line_end_reached = 0;

	while(reason_phrase_bytes_read < largest_reason_phrase)
	{
		byte_read = read_from_stream(rs, &byte, 1, &error);
		if(byte_read == 0 || error != 0)
			return -1;

		if(byte == '\n' && last_char_CR)
		{
			status_line_end_reached = 1;
			char* CRLF = "\r\n";
			unread_from_stream(rs, CRLF, 2);
			break;
		}

		last_char_CR = (byte == '\r');
		reason_phrase_bytes_read++;
	}

	if(!status_line_end_reached)
		return -1;

	return 0;
}

int serialize_http_status(stream* ws, const int* s)
{
	int error = 0;

	const char* status_reason_string = get_http_status_line((*s));
	if(status_reason_string == NULL) // this check ensures that it is a valid status code
		return -1;

	char SP = ' ';
	write_to_stream(ws, &SP, 1, &error);
	if(error)
		return -1;

	write_to_stream(ws, status_reason_string, strlen(status_reason_string), &error);
	if(error)
		return -1;

	return 0;
}