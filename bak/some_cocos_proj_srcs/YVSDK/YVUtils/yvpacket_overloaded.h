#ifndef _YVPACKET_OVER
#define _YVPACKET_OVER
#include "yvpacket_sdk.h"

static void parser_set_cstring(YV_PARSER parser, unsigned char cmdId, const char* value)
{
	parser_set_string(parser, cmdId, (char *)value);
}

static void parser_get_object(YV_PARSER parser, unsigned char cmdId, YV_PARSER object)
{
	parser_get_object(parser, cmdId, object, 0);
}
static unsigned char parser_get_uint8(YV_PARSER parser, unsigned char cmdId)
{
	return parser_get_uint8(parser, cmdId,  0);
}
static unsigned int parser_get_uint32(YV_PARSER parser, unsigned char cmdId)
{
	return parser_get_uint32(parser, cmdId,  0);
}
static int parser_get_integer(YV_PARSER parser, unsigned char cmdId)
{
	return parser_get_integer(parser, cmdId,  0);
}
static const char* parser_get_string(YV_PARSER parser, unsigned char cmdId)
{
	return parser_get_string(parser, cmdId,  0);
}
static char* parser_get_buffer(YV_PARSER parser, unsigned char cmdId, int& len)
{
	return parser_get_buffer(parser, cmdId, &len, 0);
}
static bool parser_is_empty(YV_PARSER parser, unsigned char cmdId)
{
	return parser_is_empty(parser, cmdId,  0);
}

#endif