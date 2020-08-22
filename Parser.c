/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2017, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <jo/jo.h>

bool                    jo_map_load_from_file(const unsigned int layer, const short depth, const char *const sub_dir, const char *const filename)
{
    char*				stream;
    char                *stream_begin;
    char*				tmp;
    int					tile_count;
    char				sprite[JO_MAX_FILENAME_LENGTH];
    char				x[JO_MAP_PARSER_BUF_SIZE];
    char				y[JO_MAP_PARSER_BUF_SIZE];
    char				attribute[JO_MAP_PARSER_BUF_SIZE];
    unsigned char       attribute_value;
    register int		i;

    if ((stream = jo_fs_read_file_in_dir(filename, sub_dir, 0)) == JO_NULL)
        return (false);
    stream_begin = stream;
    JO_ZERO(tile_count);
    for (tmp = stream; *tmp; ++tmp)
        if (*tmp == '\n')
            ++tile_count;
    if (tile_count <= 0)
    {
        jo_free(stream_begin);
        return (false);
    }
    if (!jo_map_create(layer, tile_count, depth))
    {
        jo_free(stream_begin);
        return (false);
    }
    while (*stream)
    {
        JO_ZERO(attribute_value);
        while (*stream && jo_tools_is_whitespace(*stream))
            ++stream;
        if (!*stream)
        {
            jo_free(stream_begin);
            return (true);
        }
        for (JO_ZERO(i); *stream && !jo_tools_is_whitespace(*stream); ++i)
        {
#ifdef JO_DEBUG
            if (i >= JO_MAX_FILENAME_LENGTH)
            {
                jo_core_error("Filename too long: %s", stream);
                jo_free(stream_begin);
                return (0);
            }
#endif
