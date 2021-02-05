#include <check.h>
#include <stdlib.h>
#include "../libcsv/include/csv.h"
#include "../libcsv/safegetline.h"
#include "../libcsv/misc.h"

char* buf = NULL;
size_t buflen = 0;
size_t linelen = 0;
FILE* _file = NULL;

/**
 * safegetline Testing
 */

void sgl_setup(void)
{
        buf = NULL;
        buflen = 0;
        linelen = 0;
        _file = NULL;
}

void sgl_teardown(void)
{
        free(buf);
        buf = NULL;
        fclose(_file);
}

void test_getline1()
{
        int ret = sgetline(_file, &buf, &buflen, &linelen);
        ck_assert_ptr_nonnull(buf);
        ck_assert_str_eq(buf, "123,456,789");
        ck_assert_int_eq(ret, 0);
        ck_assert_uint_eq(linelen, 11);
        ck_assert_uint_eq(buflen, BUFFER_FACTOR);

        ret = sgetline(_file, &buf, &buflen, &linelen);
        ck_assert_str_eq(buf, "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345");
        ck_assert_uint_eq(linelen, strlen("012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345"));
        ck_assert_int_eq(ret, 0);
        printf("BUFFER SIZE: %lu\n", buflen);
        //ck_assert_uint_eq(buflen, BUFFER_FACTOR);

        ret = sgetline(_file, &buf, &buflen, &linelen);
        ck_assert_int_eq(ret, EOF);
}

START_TEST(test_safegetline_lf)
{
        _file = fopen("test_lf.txt", "r");
        if (!_file) {
                perror("test_lf.txt");
                exit(EXIT_FAILURE);
        }
        test_getline1();
}
END_TEST

START_TEST(test_safegetline_cr)
{
        _file = fopen("test_cr.txt", "r");
        if (!_file) {
                perror("test_cr.txt");
                exit(EXIT_FAILURE);
        }
        test_getline1();
}
END_TEST

START_TEST(test_safegetline_crlf)
{
        _file = fopen("test_crlf.txt", "r");
        if (!_file) {
                perror("test_crlf.txt");
                exit(EXIT_FAILURE);
        }
        test_getline1();
}
END_TEST

START_TEST(test_safegetline_notrail)
{
        _file = fopen("test_notrail.txt", "r");
        if (!_file) {
                perror("test_notrail.txt");
                exit(EXIT_FAILURE);
        }
        test_getline1();
}
END_TEST

START_TEST(test_safegetline_long)
{
        _file = fopen("test_long.txt", "r");
        if (!_file) {
                perror("test_long.txt");
                exit(EXIT_FAILURE);
        }
        int ret = sgetline(_file, &buf, &buflen, &linelen);
        ck_assert_ptr_nonnull(buf);
        ck_assert_str_eq(buf, "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
        ck_assert_uint_eq(linelen, strlen("012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"));
        ck_assert_int_eq(ret, 0);
        ck_assert_uint_gt(buflen, BUFFER_FACTOR);

        ret = sgetline(_file, &buf, &buflen, &linelen);
        ck_assert_int_eq(ret, EOF);
}
END_TEST


struct csv_reader* reader = NULL;
struct csv_record* record = NULL;

void parse_setup(void)
{
        reader = csv_reader_new();
        record = csv_record_new();
}

void parse_teardown(void)
{
        csv_reader_free(reader);
}



/**
 * Parse Testing
 */


void file_setup(void)
{
        parse_setup();
        csv_reader_open(reader, "basic.csv");
}

START_TEST(test_parse_rfc)
{
        int ret = 0;
        ret = csv_parse(reader, record, "123,456,789,,");
        ck_assert_uint_eq(record->size, 5);
        ck_assert_str_eq(record->fields[0], "123");
        ck_assert_str_eq(record->fields[1], "456");
        ck_assert_str_eq(record->fields[2], "789");
        ck_assert_str_eq(record->fields[3], "");
        ck_assert_str_eq(record->fields[4], "");

        ret = csv_parse(reader, record, "\"abc\",\"d,ef\",\"ghi\",\"\"");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "d,ef");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        ret = csv_parse(reader, record, "abc,\"de\nf\",ghi,");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\nf");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        ret = csv_parse(reader, record, "abc,\"de\"\"f\",ghi,");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\"f");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        uint rows = csv_reader_row_count(reader);
        uint breaks = csv_reader_embedded_breaks(reader);

        ck_assert_uint_eq(rows, 4);
        ck_assert_uint_eq(breaks, 1);

}
END_TEST

START_TEST(test_parse_weak)
{
        int ret = 0;
        reader->quotes = QUOTE_WEAK;
        ret = csv_parse(reader, record, "123,456,789,,");
        ck_assert_uint_eq(record->size, 5);
        ck_assert_str_eq(record->fields[0], "123");
        ck_assert_str_eq(record->fields[1], "456");
        ck_assert_str_eq(record->fields[2], "789");
        ck_assert_str_eq(record->fields[3], "");
        ck_assert_str_eq(record->fields[4], "");

        ret = csv_parse(reader, record, "\"abc\",\"d,ef\",\"ghi\",\"\"");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "d,ef");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        ret = csv_parse(reader, record, "abc,\"de\nf\",ghi,");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\nf");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        ret = csv_parse(reader, record, "abc,\"de\"\"f\",ghi,");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\"\"f");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        uint rows = csv_reader_row_count(reader);
        uint breaks = csv_reader_embedded_breaks(reader);

        ck_assert_uint_eq(rows, 4);
        ck_assert_uint_eq(breaks, 1);
}
END_TEST

START_TEST(test_parse_none)
{
        int ret = 0;
        reader->quotes = QUOTE_NONE;
        ret = csv_parse(reader, record, "123,456,789,,");
        ck_assert_uint_eq(record->size, 5);
        ck_assert_str_eq(record->fields[0], "123");
        ck_assert_str_eq(record->fields[1], "456");
        ck_assert_str_eq(record->fields[2], "789");
        ck_assert_str_eq(record->fields[3], "");
        ck_assert_str_eq(record->fields[4], "");

        ret = csv_parse(reader, record, "\"abc\",\"d,ef\",\"ghi\",\"\"");
        ck_assert_uint_eq(record->size, 5);
        ck_assert_str_eq(record->fields[0], "\"abc\"");
        ck_assert_str_eq(record->fields[1], "\"d");
        ck_assert_str_eq(record->fields[2], "ef\"");
        ck_assert_str_eq(record->fields[3], "\"ghi\"");
        ck_assert_str_eq(record->fields[4], "\"\"");

        ret = csv_parse(reader, record, "abc,\"de\nf\",ghi,");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "\"de\nf\"");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        ret = csv_parse(reader, record, "abc,\"de\"\"f\",ghi,");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "\"de\"\"f\"");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        uint rows = csv_reader_row_count(reader);
        uint breaks = csv_reader_embedded_breaks(reader);

        ck_assert_uint_eq(rows, 4);
        ck_assert_uint_eq(breaks, 1);
}
END_TEST

START_TEST(test_parse_ldrfc)
{
        int ret = 0;
        strcpy(reader->delimiter, "~^_");
        ret = csv_parse(reader, record, "123~^_456~^_789~^_~^_");
        ck_assert_uint_eq(record->size, 5);
        ck_assert_str_eq(record->fields[0], "123");
        ck_assert_str_eq(record->fields[1], "456");
        ck_assert_str_eq(record->fields[2], "789");
        ck_assert_str_eq(record->fields[3], "");
        ck_assert_str_eq(record->fields[4], "");

        ret = csv_parse(reader, record, "\"abc\"~^_\"d~^_ef\"~^_\"ghi\"~^_\"\"");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "d~^_ef");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        ret = csv_parse(reader, record, "abc~^_\"de\nf\"~^_ghi~^_");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\nf");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        ret = csv_parse(reader, record, "abc~^_\"de\"\"f\"~^_ghi~^_");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\"f");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        uint rows = csv_reader_row_count(reader);
        uint breaks = csv_reader_embedded_breaks(reader);

        ck_assert_uint_eq(rows, 4);
        ck_assert_uint_eq(breaks, 1);
}
END_TEST

START_TEST(test_parse_ldweak)
{
        int ret = 0;
        reader->quotes = QUOTE_WEAK;
        strcpy(reader->delimiter, "~^_");
        ret = csv_parse(reader, record, "123~^_456~^_789~^_~^_");
        ck_assert_uint_eq(record->size, 5);
        ck_assert_str_eq(record->fields[0], "123");
        ck_assert_str_eq(record->fields[1], "456");
        ck_assert_str_eq(record->fields[2], "789");
        ck_assert_str_eq(record->fields[3], "");
        ck_assert_str_eq(record->fields[4], "");

        ret = csv_parse(reader, record, "\"abc\"~^_\"d~^_ef\"~^_\"ghi\"~^_\"\"");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "d~^_ef");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        ret = csv_parse(reader, record, "abc~^_\"de\nf\"~^_ghi~^_");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\nf");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        ret = csv_parse(reader, record, "abc~^_\"de\"\"f\"~^_ghi~^_");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\"\"f");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        uint rows = csv_reader_row_count(reader);
        uint breaks = csv_reader_embedded_breaks(reader);

        ck_assert_uint_eq(rows, 4);
        ck_assert_uint_eq(breaks, 1);
}
END_TEST

START_TEST(test_parse_ldnone)
{
        int ret = 0;
        reader->quotes = QUOTE_NONE;
        strcpy(reader->delimiter, "~^_");
        ret = csv_parse(reader, record, "123~^_456~^_789~^_~^_");
        ck_assert_uint_eq(record->size, 5);
        ck_assert_str_eq(record->fields[0], "123");
        ck_assert_str_eq(record->fields[1], "456");
        ck_assert_str_eq(record->fields[2], "789");
        ck_assert_str_eq(record->fields[3], "");
        ck_assert_str_eq(record->fields[4], "");

        ret = csv_parse(reader, record, "\"abc\"~^_\"d~^_ef\"~^_\"ghi\"~^_\"\"");
        ck_assert_uint_eq(record->size, 5);
        ck_assert_str_eq(record->fields[0], "\"abc\"");
        ck_assert_str_eq(record->fields[1], "\"d");
        ck_assert_str_eq(record->fields[2], "ef\"");
        ck_assert_str_eq(record->fields[3], "\"ghi\"");
        ck_assert_str_eq(record->fields[4], "\"\"");

        ret = csv_parse(reader, record, "abc~^_\"de\nf\"~^_ghi~^_");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "\"de\nf\"");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        ret = csv_parse(reader, record, "abc~^_\"de\"\"f\"~^_ghi~^_");
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "\"de\"\"f\"");
        ck_assert_str_eq(record->fields[2], "ghi");
        ck_assert_str_eq(record->fields[3], "");

        uint rows = csv_reader_row_count(reader);
        uint breaks = csv_reader_embedded_breaks(reader);

        ck_assert_uint_eq(rows, 4);
        ck_assert_uint_eq(breaks, 1);
}
END_TEST

START_TEST(test_file_rfc)
{
        int ret = 0;

        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "123");
        ck_assert_str_eq(record->fields[1], "456");
        ck_assert_str_eq(record->fields[2], "789");

        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "d|ef");
        ck_assert_str_eq(record->fields[2], "ghi");

        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\nf");
        ck_assert_str_eq(record->fields[2], "ghi");

        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\"f");
        ck_assert_str_eq(record->fields[2], "ghi");

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, EOF);

        uint rows = csv_reader_row_count(reader);
        uint breaks = csv_reader_embedded_breaks(reader);

        ck_assert_uint_eq(rows, 4);
        ck_assert_uint_eq(breaks, 1);
}
END_TEST

START_TEST(test_file_weak)
{
        int ret = 0;

        reader->quotes = QUOTE_WEAK;
        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "123");
        ck_assert_str_eq(record->fields[1], "456");
        ck_assert_str_eq(record->fields[2], "789");

        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "d|ef");
        ck_assert_str_eq(record->fields[2], "ghi");

        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\nf");
        ck_assert_str_eq(record->fields[2], "ghi");

        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\"\"f");
        ck_assert_str_eq(record->fields[2], "ghi");

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, EOF);

        uint rows = csv_reader_row_count(reader);
        uint breaks = csv_reader_embedded_breaks(reader);

        ck_assert_uint_eq(rows, 4);
        ck_assert_uint_eq(breaks, 1);
}
END_TEST

START_TEST(test_file_none)
{
        int ret = 0;

        reader->quotes = QUOTE_NONE;
        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "123");
        ck_assert_str_eq(record->fields[1], "456");
        ck_assert_str_eq(record->fields[2], "789");

        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 4);
        ck_assert_str_eq(record->fields[0], "\"abc\"");
        ck_assert_str_eq(record->fields[1], "\"d");
        ck_assert_str_eq(record->fields[2], "ef\"");
        ck_assert_str_eq(record->fields[3], "\"ghi\"");

        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 2);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "\"de");

        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 2);
        ck_assert_str_eq(record->fields[0], "f\"");
        ck_assert_str_eq(record->fields[1], "ghi");

        ret = csv_get_record(reader, record);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "\"de\"\"f\"");
        ck_assert_str_eq(record->fields[2], "ghi");

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, EOF);

        uint rows = csv_reader_row_count(reader);
        uint breaks = csv_reader_embedded_breaks(reader);

        ck_assert_uint_eq(rows, 5);
        ck_assert_uint_eq(breaks, 0);
}
END_TEST

START_TEST(test_multiple_eol)
{
        int ret = 0;

        reader->failsafe_mode = true;

        csv_reader_open(reader, "test_multi_eol.txt");
        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, CSV_GOOD);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "123");
        ck_assert_str_eq(record->fields[1], "4\n5\n6");
        ck_assert_str_eq(record->fields[2], "789");

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, EOF);

        uint rows = csv_reader_row_count(reader);
        uint breaks = csv_reader_embedded_breaks(reader);

        ck_assert_uint_eq(rows, 1);
        ck_assert_uint_eq(breaks, 2);
}


START_TEST(test_realloc_append)
{
        int ret = 0;

        reader->failsafe_mode = true;

        csv_reader_open(reader, "test_realloc_append.txt");
        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, CSV_GOOD);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "aaa");
        ck_assert_str_eq(record->fields[1], "1234567890\n1234567890123456789012345678901234567890123456789012345678901234567890\n1234567890123456789012345678901234567890123456789012345678901234567890");
        ck_assert_str_eq(record->fields[2], "ccc");

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, CSV_GOOD);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "def");
        ck_assert_str_eq(record->fields[2], "ghi");

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, EOF);

        uint rows = csv_reader_row_count(reader);
        uint breaks = csv_reader_embedded_breaks(reader);

        ck_assert_uint_eq(rows, 2);
        ck_assert_uint_eq(breaks, 2);
}


START_TEST(test_fs_eof)
{
        int ret = 0;

        reader->failsafe_mode = true;

        csv_reader_open(reader, "test_fs_eof.txt");
        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, CSV_RESET);

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, CSV_RESET);

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, CSV_GOOD);
        ck_assert_str_eq(record->fields[0], "\"");

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, EOF);
}

START_TEST(test_fs_max)
{
        int ret = 0;

        reader->failsafe_mode = true;

        csv_reader_open(reader, "test_fs_max.txt");
        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, CSV_RESET);

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, CSV_RESET);

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, CSV_GOOD);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "\"def");

        while ((ret = csv_get_record(reader, record)) == CSV_GOOD);

        uint rows = csv_reader_row_count(reader);
        ck_assert_uint_eq(rows, 41);
}

START_TEST(test_fs_weak)
{
        int ret = 0;

        reader->failsafe_mode = true;

        csv_reader_open(reader, "test_fs_weak.txt");
        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, CSV_RESET);

        ret = csv_get_record(reader, record);
        ck_assert_int_eq(ret, CSV_GOOD);
        ck_assert_uint_eq(record->size, 3);
        ck_assert_str_eq(record->fields[0], "abc");
        ck_assert_str_eq(record->fields[1], "de\"f");
        ck_assert_str_eq(record->fields[2], "ghi");
}

//START_TEST(test_weak_trailing)
//{
//        int ret = 0;
//        reader->quotes = QUOTE_WEAK;
//
//        csv_reader_open(reader, "test_weak_trailing.txt");
//
//        ret = csv_get_record(reader, record);
//        ck_assert_int_eq(ret, CSV_GOOD);
//        ck_assert_uint_eq(record->size, 3);
//        ck_assert_str_eq(record->fields[0], "abc");
//        ck_assert_str_eq(record->fields[1], "def");
//        ck_assert_str_eq(record->fields[2], "ghi");
//}

Suite* csv_reader_suite(void)
{
        Suite* s;
        s = suite_create("Reader");

        TCase* tc_sgl = tcase_create("safegetline");
        tcase_add_checked_fixture(tc_sgl, sgl_setup, sgl_teardown);
        tcase_add_test(tc_sgl, test_safegetline_lf);
        tcase_add_test(tc_sgl, test_safegetline_cr);
        tcase_add_test(tc_sgl, test_safegetline_crlf);
        tcase_add_test(tc_sgl, test_safegetline_notrail);
        tcase_add_test(tc_sgl, test_safegetline_long);
        suite_add_tcase(s, tc_sgl);

        TCase* tc_parse_rfc = tcase_create("rfc4180");
        tcase_add_checked_fixture(tc_parse_rfc, parse_setup, parse_teardown);
        tcase_add_test(tc_parse_rfc, test_parse_rfc);
        suite_add_tcase(s, tc_parse_rfc);

        TCase* tc_parse_weak = tcase_create("weak");
        tcase_add_checked_fixture(tc_parse_weak, parse_setup, parse_teardown);
        tcase_add_test(tc_parse_weak, test_parse_weak);
        suite_add_tcase(s, tc_parse_weak);

        TCase* tc_parse_none = tcase_create("none");
        tcase_add_checked_fixture(tc_parse_none, parse_setup, parse_teardown);
        tcase_add_test(tc_parse_none, test_parse_none);
        suite_add_tcase(s, tc_parse_none);

        TCase* tc_parse_ldrfc = tcase_create("ld_rfc4180");
        tcase_add_checked_fixture(tc_parse_ldrfc, parse_setup, parse_teardown);
        tcase_add_test(tc_parse_ldrfc, test_parse_ldrfc);
        suite_add_tcase(s, tc_parse_ldrfc);

        TCase* tc_parse_ldweak = tcase_create("ld_weak");
        tcase_add_checked_fixture(tc_parse_ldweak, parse_setup, parse_teardown);
        tcase_add_test(tc_parse_ldweak, test_parse_ldweak);
        suite_add_tcase(s, tc_parse_ldweak);

        TCase* tc_parse_ldnone = tcase_create("ld_none");
        tcase_add_checked_fixture(tc_parse_ldnone, parse_setup, parse_teardown);
        tcase_add_test(tc_parse_ldnone, test_parse_ldnone);
        suite_add_tcase(s, tc_parse_ldnone);

        TCase* tc_file_rfc = tcase_create("rfc4180");
        tcase_add_checked_fixture(tc_file_rfc, file_setup, parse_teardown);
        tcase_add_test(tc_file_rfc, test_file_rfc);
        suite_add_tcase(s, tc_file_rfc);

        TCase* tc_file_weak = tcase_create("weak");
        tcase_add_checked_fixture(tc_file_weak, file_setup, parse_teardown);
        tcase_add_test(tc_file_weak, test_file_weak);
        suite_add_tcase(s, tc_file_weak);

        TCase* tc_file_none = tcase_create("none");
        tcase_add_checked_fixture(tc_file_none, file_setup, parse_teardown);
        tcase_add_test(tc_file_none, test_file_none);
        suite_add_tcase(s, tc_file_none);

        TCase* tc_multi_eol = tcase_create("multi_eol");
        tcase_add_checked_fixture(tc_multi_eol, parse_setup, parse_teardown);
        tcase_add_test(tc_multi_eol, test_multiple_eol);
        suite_add_tcase(s, tc_multi_eol);

        TCase* tc_realloc_append = tcase_create("realloc_append");
        tcase_add_checked_fixture(tc_realloc_append, parse_setup, parse_teardown);
        tcase_add_test(tc_realloc_append, test_realloc_append);
        suite_add_tcase(s, tc_realloc_append);

        TCase* tc_failsafe_eof = tcase_create("failsafe_eof");
        tcase_add_checked_fixture(tc_failsafe_eof, parse_setup, parse_teardown);
        tcase_add_test(tc_failsafe_eof, test_fs_eof);
        suite_add_tcase(s, tc_failsafe_eof);

        TCase* tc_failsafe_max = tcase_create("failsafe_max");
        tcase_add_checked_fixture(tc_failsafe_max, parse_setup, parse_teardown);
        tcase_add_test(tc_failsafe_max, test_fs_max);
        suite_add_tcase(s, tc_failsafe_max);

        TCase* tc_failsafe_weak = tcase_create("failsafe_weak");
        tcase_add_checked_fixture(tc_failsafe_weak, parse_setup, parse_teardown);
        tcase_add_test(tc_failsafe_weak, test_fs_weak);
        suite_add_tcase(s, tc_failsafe_weak);

        //TCase* tc_weak_trailing = tcase_create("failsafe_weak");
        //tcase_add_checked_fixture(tc_weak_trailing, parse_setup, parse_teardown);
        //tcase_add_test(tc_weak_trailing, test_weak_trailing);
        //suite_add_tcase(s, tc_weak_trailing);

        return s;
}

int main(void)
{
        int number_failed;
        Suite* s = csv_reader_suite();
        SRunner* sr = srunner_create(s);
        srunner_set_fork_status (sr, CK_NOFORK);

        srunner_run_all(sr, CK_VERBOSE);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

