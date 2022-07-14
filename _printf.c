#include "main.h"

void check(va_list args, buffer *output);
int run_printf(const char *format, va_list args, buffer *output);
int _printf(const char *format, ...);

/**
 * _printf - Gives output of formated string
 * @format: Character string and has more directives
 * Return: Number of characters to be printed
 **/
int _printf(const char *format, ...)
{
	buffer *output;
	va_list args;
	int ret;

	if (format == '\0')
		return (-1);

	output = init_buffer();
	if (output == '\0')
		return (-1);
	va_start(args, format);
	ret = run_printf(format, args, output);
	return (ret);
}

/**
 * check - performs check operations for _printf
 * @args: va_list argumrnts for _printf
 * @output: buffer struct
 */
void check(va_list args, buffer *output)
{
	va_end(args);
	write(1, output->start, output->len);
	free_buffer(output);
}

/**
 * run_printf - Reads through the format string for _
 * @format: Character string to print - may contain d
 * @output: A buffer struct containing a buffer.
 * @args: A va_list of arguments.
 *
 * Return: Number of characters stored
 */
int run_printf(const char *format, va_list args, buffer *output)
{
	int i, wid, prec, ret = 0;
	char tmp;
	unsigned char flags, len;
	unsigned int (*f)(va_list, buffer *,
			unsigned char, int, int, unsigned char);

	for (i = 0; *(format + i); i++)
	{
		len = 0;
		if (*(format + i) == '%')
		{
			tmp = 0;
			flags = handle_flags(format + i + 1, &tmp);
			wid = handle_width(args, format + i + tmp + 1, &tmp);
			prec = handle_precision(args, format + i + tmp + 1,
					&tmp);
			len = handle_length(format + i + tmp + 1, &tmp);

			f = handle_specifiers(format + i + tmp + 1);
			if (f != NULL)
			{
				i += tmp + 1;
				ret += f(args, output, flags, wid, prec, len);
				continue;
			}
			else if (*(format + i + tmp + 1) == '\0')
			{
				ret = -1;
				break;
			}
		}
		ret += _memcpy(output, (format + i), 1);
		i += (len != 0) ? 1 : 0;
	}
	check(args, output);
	return (ret);
}

