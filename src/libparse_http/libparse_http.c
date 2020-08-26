/* Copyright (C) 2020 The uOFW team
   See the file COPYING for copying permission.
*/

#include <common_imp.h>
#include <threadman_user.h>

SCE_MODULE_INFO("SceParseHTTPheader_Library", SCE_MODULE_ATTR_EXCLUSIVE_LOAD | SCE_MODULE_ATTR_EXCLUSIVE_START, 1, 1);
SCE_SDK_VERSION(SDK_VERSION);

#define SCE_PARSE_HTTP_NOT_FOUND        0x80432025 // lui $s2, 0x8043 -> ori $v0, $s2, 0x2025
#define SCE_PARSE_HTTP_INVALID_RESPONSE 0x80432060 // lui $a0, 0x8043 -> ori $v0, $a0, 0x2060

// Internal function prototyes
s32 sub_000003A0(u8 *status_line, u32 line_len, s32 *http_major_ver, s32 *http_minor_ver);
s32 sub_00000510(u8 *status_line, u32 line_len, s32 *response_code);
s32 sub_0000061C(u8 *status_line, u32 line_len, u8 **reason_phrase, u32 *phrase_len);
s32 sub_000006CC(u8 *unk, u8 *status_line, s32 len);

// Subroutine sceParseHttp_8077A433 - Address 0x00000000
s32 sceParseHttpStatusLine(const u8 *status_line, u32 line_len, u32 *http_major_ver, u32 *http_minor_ver, u32 *response_code, const u8 **reason_phrase, u32 *phrase_len)
{	
	s32 check = sceKernelCheckThreadStack();
	s32 ret = 0x80410000;
	if ((0x3df < check) && (status_line != 0))
	{
		if ((http_major_ver == 0 || http_minor_ver == 0) || ((response_code == 0 || reason_phrase == 0 || (phrase_len == 0))))
			ret = 0x80410005;
		else
		{
			ret = sub_000003A0(status_line, line_len, http_major_ver, http_minor_ver);
			line_len = line_len - ret;
			if (ret >= 0)
			{
				check = status_line + ret;
				ret = sub_00000510(check, line_len, response_code);
				
				if (ret >= 0)
				{
					check = check + ret;
					ret = sub_0000061C(check, line_len - ret, reason_phrase, phrase_len);
					if (ret >= 0)
						ret = (check + ret) - (s32)status_line;
				}
			}
		}
	}
	
	return ret;
}

// Subroutine sceParseHttp_AD7BFDEF - Address 0x00000118
s32 sceParseHttpResponseHeader(const u8 *header, u32 header_len, const u8 *field_name, const u8 **field_value, u32 *value_len)
{
	return 0;
}

// Subroutine sub_000003A0 - Address 0x000003A0
s32 sub_000003A0(u8 *status_line, u32 line_len, s32 *http_major_ver, s32 *http_minor_ver)
{
	s32 var0;
	u8 *var1;
	*http_major_ver = 0;
	*http_minor_ver = 0;

	s32 ret = SCE_PARSE_HTTP_INVALID_RESPONSE;

	if (line_len > 8)
	{
		var0 = sub_000006CC(status_line, (u8 *)"HTTP/", 5);

		if (var0 == 0)
		{
			if ((*(u8 *)((s32)status_line[5] + 1) & 4) != 0)
			{
				var1 = status_line + 5;
				ret = *http_major_ver;
				var0 = 5;

				while(1)
				{
					*http_major_ver = ret * 10 + (s32)*var1 + -0x30;
					var1 = status_line + var0 + 1;

					if ((*(u8 *)((s32)*var1 + 1) & 4) == 0)
						break;

					ret = *http_major_ver;
					var0 = var0 + 1;
				}

				ret = SCE_PARSE_HTTP_INVALID_RESPONSE;

				if ((s32)*var1 == 0x2e)
				{
					var1 = status_line + var0 + 2;

					if ((*(u8 *)((s32)*var1 + 1) & 4) != 0)
					{
						ret = *http_minor_ver;
						var0 = var0 + 2;

						while(1)
						{
							*http_minor_ver = ret * 10 + (s32)*var1 + -0x30;
							var1 = status_line + var0 + 1;

							if ((*(u8 *)((s32)*var1 + 1) & 4) == 0)
								break;

							ret = *http_minor_ver;
							var0 = var0 + 1;
						}

						ret = SCE_PARSE_HTTP_INVALID_RESPONSE;

						if ((s32)*var1 == 0x20)
							ret = var0 + 2;
					}
				}
			}
		}
	}

	return ret;
}

// Subroutine sub_00000510 - Address 0x00000510
s32 sub_00000510(u8 *status_line, u32 line_len, s32 *response_code)
{
	s32 ret = SCE_PARSE_HTTP_INVALID_RESPONSE;

	if (3 < line_len)
	{
		ret = SCE_PARSE_HTTP_INVALID_RESPONSE;

		if (((*(u8 *)((s32)*status_line + 1) & 4) != 0) && ((*(u8 *)((s32)status_line[1] + 1) & 4) != 0))
		{
			if ((*(u8 *)((s32)status_line[2] + 1) & 4) != 0)
			{
				ret = 3;
				*response_code = (s32)*status_line * 100 + (s32)status_line[1] * 10 + (s32)status_line[2] + -0x14d0;
			}
		}
	}

	return ret;
}

// Subroutine sub_0000061C - Address 0x0000061C
s32 sub_0000061C(u8 *status_line, u32 line_len, u8 **reason_phrase, u32 *phrase_len)
{
	u8 var0 = 0;
	u8 *var1 = status_line;
	u32 var2 = 0;
	
	while(1)
	{
		if (*var1 == '\n')
		{
			if (var2 != 0)
				var0 = var1[-1] == '\r';
			
			if (var0)
				*phrase_len = var2 - 1;
			else
				*phrase_len = var2;
				
			*reason_phrase = status_line;
			return var2 + 1;
		}
		
		var2 = var2 + 1;
		if (line_len < var2)
			break;
			
		var1 = status_line + var2;
	}
	
	return SCE_PARSE_HTTP_INVALID_RESPONSE;
}

// Subroutine sub_000006CC - Address 0x000006CC
s32 sub_000006CC(u8 *unk, u8 *status_line, s32 len)
{
	u8 var0;
	s32 ret = 0;
	u8 *var2;
	u8 var3;

	if (unk == (u8 *)0x0 || status_line == (u8 *)0x0)
	{
		if ((unk != status_line) && (ret = -1, unk != (u8 *)0x0))
			ret = 1;
	}
	else
	{
		len = len + -1;

		if (-1 < len)
		{
			var3 = *unk;
			var2 = status_line + 1;
			if (var3 == *status_line)
			{
				do
				{
					len = len + -1;
					unk = unk + 1;

					if (var3 == 0)
						return 0;

					if (len < 0)
						return 0;

					var0 = *var2;
					var3 = *unk;
					var2 = var2 + 1;
				} while (var3 == var0);
			}

			if (len < 0)
				ret = 0;
			else
				ret = (s32)*unk - (s32)var2[-1];
		}
	}

	return ret;
}
