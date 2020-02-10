/******************************************************************************
 *	Copyright (C) 2020	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	BSD-2-Clause			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <csv.h>

#define ALX_NO_PREFIX
#include <libalx/alx/data-structures/bst.h>
#include <libalx/alx/data-structures/node.h>
#include <libalx/alx/data-structures/llist.h>
#include <libalx/base/compiler/unused.h>
#include <libalx/base/errno/error.h>
#include <libalx/base/libgen/basename.h>
#include <libalx/base/stdio/printf/sbprintf.h>
#include <libalx/base/stdio/printf/snprintfs.h>
#include <libalx/base/stdlib/strto/strtof_s.h>
#include <libalx/base/stdlib/strto/strtoi_s.h>


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/

/******************************************************************************
 ******* enum / struct / union ************************************************
 ******************************************************************************/
enum	Data_Frame_e {
	DF_SYMBOLING,		DF_NORM_LOSSES,		DF_MAKE,
	DF_FUEL_TYPE,		DF_ASPIRATION,		DF_DOORS,
	DF_STYLE,		DF_DRIVE_WH,		DF_ENGINE_POS,
	DF_WHEEL_BASE,		DF_LENGTH,		DF_WIDTH,
	DF_HEIGHT,		DF_CURB_WEIGHT,		DF_ENGINE_TYPE,
	DF_CYLINDERS,		DF_ENGINE_SIZE,		DF_FUEL_SYSTEM,
	DF_BORE,		DF_STROKE,		DF_COMPRESSION_RATIO,
	DF_HP,			DF_PEAK_RPM,		DF_CITY_MPG,
	DF_HIWAY_MPG,		DF_PRICE
};

struct	Data_Frame {
	int32_t	symboling;
	int32_t	norm_losses;
	int	make;
	int	fuel_type;
	int	aspiration;
	int	doors;
	int	style;
	int	drive_wh;
	int	engine_pos;
	double	wheel_base;
	double	length;
	double	width;
	double	height;
	int32_t	curb_weight;
	int	engine_type;
	int	cylinders;
	int32_t	engine_size;
	int	fuel_system;
	double	bore;
	double	stroke;
	double	compression_ratio;
	int32_t	hp;
	int32_t	peak_rpm;
	int32_t	city_mpg;
	int32_t	hiway_mpg;
	int32_t	price;
	bool	error;
};

struct	Text_Values {
	struct Alx_BST	*make;
	struct Alx_BST	*fuel_type;
	struct Alx_BST	*aspiration;
	struct Alx_BST	*doors;
	struct Alx_BST	*style;
	struct Alx_BST	*drive_wh;
	struct Alx_BST	*engine_pos;
	struct Alx_BST	*engine_type;
	struct Alx_BST	*cylinders;
	struct Alx_BST	*fuel_system;
};

struct	Data {
	struct Text_Values	values;
	struct Alx_LinkedList	*rows;
	struct Data_Frame	row;
	ptrdiff_t		nrow;
	ptrdiff_t		ncol;
};


/******************************************************************************
 ******* static variables *****************************************************
 ******************************************************************************/
static	const char	*const url	= "https://s3-api.us-geo.objectstorage.softlayer.net/cf-courses-data/CognitiveClass/DA0101EN/auto.csv";
/*static	const char	*const headers[]	= {
	"symboling",		"normalized-losses",	"make",
	"fuel-type",		"aspiration",		"num-of-doors",
	"body-style",		"drive-wheels",		"engine-location",
	"wheel-base",		"length",		"width",
	"height",		"curb-weight",		"engine-type",
	"num-of-cylinders",	"engine-size",		"fuel-system",
	"bore",			"stroke",		"compression-ratio",
	"horsepower",		"peak-rpm",		"city-mpg",
	"highway-mpg",		"price"			};*/


/******************************************************************************
 ******* static prototypes ****************************************************
 ******************************************************************************/
static
void	wget		(const char *url);
static
void	get_fname	(char fname[static restrict FILENAME_MAX],
			 const char *restrict url);
static
void	less_data	(const char *fname);

static
void	parse_field	(void *restrict parsed, size_t size,
			 void *restrict data);
static
void	parse_row	(int c, void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
int	main	(void)
{
	char			file[FILENAME_MAX];
	struct Data		data;
	struct csv_parser	parser;
	FILE			*fp;
	size_t			n;
	char			buf[BUFSIZ];
	int			status;

	status	= 0;

	wget(url);
	get_fname(file, url);

	if (alx_llist_init(&data.rows))
		errorx(EXIT_FAILURE, "llist_init()");

	if (csv_init(&parser, CSV_STRICT_FINI | CSV_APPEND_NULL)) {
		perrorx("csv_init()");
		status	= EXIT_FAILURE;
		goto out_csv;
	}

	fp	= fopen(file, "r");
	if (!fp) {
		perrorx(file);
		status	= EXIT_FAILURE;
		goto out_file;
	}

	data.nrow	= 0;
	data.ncol	= 0;
	do {
		n	= fread(buf, 1, ARRAY_SIZE(buf), fp);
		if (csv_parse(&parser, buf, n, &parse_field, &parse_row,
								&data) < n) {
			perrorx("csv_parse()");
			status	= EXIT_FAILURE;
			goto out_parse;
		}
	} while (n);

	



	less_data(file);

	return	0;

out_parse:
	fclose(fp);
out_file:
	csv_free(&parser);
out_csv:
	alx_llist_deinit(data.rows);

	return	status;
}


/******************************************************************************
 ******* static function definitions ******************************************
 ******************************************************************************/
static
void	wget		(const char *url)
{
	char	cmd[_POSIX_ARG_MAX];

	if (sbprintf(cmd, NULL, "wget -N %s", url))
		errorx(EXIT_FAILURE, url);
	if (system(cmd))
		errorx(EXIT_FAILURE, cmd);
}

static
void	get_fname	(char fname[static restrict FILENAME_MAX],
			 const char *restrict url)
{

	if (basename_s(fname, url))
		errorx(EXIT_FAILURE, url);
}

static
void	less_data	(const char *fname)
{
	char	cmd[_POSIX_ARG_MAX];

	if (sbprintf(cmd, NULL, "less %s", fname))
		errorx(EXIT_FAILURE, fname);
	if (system(cmd))
		errorx(EXIT_FAILURE, cmd);
}

/*
static
void	prn_data	(const char *fname)
{
	FILE	*less;
	char	cmd[_POSIX_ARG_MAX];

	less	= popen("less -S", "w");

	fprintf("
	for (ptrdiff_t i = 1; i < FIELDS; i++) {
		printf("



	if (sbprintf(cmd, NULL, "less %s", fname))
		errorx(EXIT_FAILURE, fname);
	if (system(cmd))
		errorx(EXIT_FAILURE, cmd);
}

static
void	prn_row		(const struct Data_Frame *row, FILE *less)
{
	char	cmd[_POSIX_ARG_MAX];

	fprintf(less, "%2i", row->symboling);
	if (row->norm_losses == -1)
		fprintf(less, " %2s", "?");
	else
		fprintf(less, " %2i", row->norm_losses);
	fprintf(less, " %15s", row->make);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	fprintf(less, "%2i", row->symboling);
	for (ptrdiff_t i = 1; i < FIELDS; i++) {
		printf("



	if (sbprintf(cmd, NULL, "less %s", fname))
		errorx(EXIT_FAILURE, fname);
	if (system(cmd))
		errorx(EXIT_FAILURE, cmd);
}
static
void	printf_less	()
{
	FILE	*fin, *fout;
	int	fd[2];

	
}

static	less_from_pipe
*/
static
void	parse_field_text	(void *restrict parsed, size_t size,
				 struct Data_Frame *restrict row,
				 struct Alx_BST *restrict values)
{
	struct Alx_Node	*tmp;
	int		key;

	if (!alx_bst_find(&tmp, values, 0, parsed)) {
		row->make = tmp->key;
	} else {
		key = values->key_max + 1;
		if (alx_bst_insert(values, key, parsed, size)) {
			row->make	= INT_MIN;
			row->error	= true;
			return;
		}
		row->make = key;
	}
}

static
void	parse_field		(void *restrict parsed, size_t size,
				 void *restrict data)
{
	struct Data	*d	= data;

	switch (d->ncol) {
	case DF_SYMBOLING:
		if (strtoi32_s(&d->row.symboling, parsed, 0, NULL)) {
			d->row.symboling	= INT_MIN;
			d->row.error		= true;
		}
		break;
	case DF_NORM_LOSSES:
		if (strtoi32_s(&d->row.norm_losses, parsed, 0, NULL)) {
			d->row.norm_losses	= INT_MIN;
			d->row.error		= true;
		}
		break;
	case DF_MAKE:
		parse_field_text(parsed, size, &d->row, d->values.make);
		break;
	case DF_FUEL_TYPE:
		parse_field_text(parsed, size, &d->row, d->values.fuel_type);
		break;
	case DF_ASPIRATION:
		parse_field_text(parsed, size, &d->row, d->values.aspiration);
		break;
	case DF_DOORS:
		parse_field_text(parsed, size, &d->row, d->values.doors);
		break;
	case DF_STYLE:
		parse_field_text(parsed, size, &d->row, d->values.style);
		break;
	case DF_DRIVE_WH:
		parse_field_text(parsed, size, &d->row, d->values.drive_wh);
		break;
	case DF_ENGINE_POS:
		parse_field_text(parsed, size, &d->row, d->values.engine_pos);
		break;
	case DF_WHEEL_BASE:
		if (strtod_s(&d->row.wheel_base, parsed, NULL)) {
			d->row.wheel_base	= nan("");
			d->row.error		= true;
		}
		break;
	case DF_LENGTH:
		if (strtod_s(&d->row.length, parsed, NULL)) {
			d->row.length		= nan("");
			d->row.error		= true;
		}
		break;
	case DF_WIDTH:
		if (strtod_s(&d->row.width, parsed, NULL)) {
			d->row.width		= nan("");
			d->row.error		= true;
		}
		break;
	case DF_HEIGHT:
		if (strtod_s(&d->row.height, parsed, NULL)) {
			d->row.height		= nan("");
			d->row.error		= true;
		}
		break;
	case DF_CURB_WEIGHT:
		if (strtoi32_s(&d->row.curb_weight, parsed, 0, NULL)) {
			d->row.curb_weight	= INT_MIN;
			d->row.error		= true;
		}
		break;
	case DF_ENGINE_TYPE:
		parse_field_text(parsed, size, &d->row, d->values.engine_type);
		break;
	case DF_CYLINDERS:
		parse_field_text(parsed, size, &d->row, d->values.cylinders);
		break;
	case DF_ENGINE_SIZE:
		if (strtoi32_s(&d->row.engine_size, parsed, 0, NULL)) {
			d->row.engine_size	= INT_MIN;
			d->row.error		= true;
		}
		break;
	case DF_FUEL_SYSTEM:
		parse_field_text(parsed, size, &d->row, d->values.fuel_system);
		break;
	case DF_BORE:
		if (strtod_s(&d->row.bore, parsed, NULL)) {
			d->row.bore		= nan("");
			d->row.error		= true;
		}
		break;
	case DF_STROKE:
		if (strtod_s(&d->row.stroke, parsed, NULL)) {
			d->row.stroke		= nan("");
			d->row.error		= true;
		}
		break;
	case DF_COMPRESSION_RATIO:
		if (strtod_s(&d->row.compression_ratio, parsed, NULL)) {
			d->row.compression_ratio = nan("");
			d->row.error		= true;
		}
		break;
	case DF_HP:
		if (strtoi32_s(&d->row.hp, parsed, 0, NULL)) {
			d->row.hp		= INT_MIN;
			d->row.error		= true;
		}
		break;
	case DF_PEAK_RPM:
		if (strtoi32_s(&d->row.peak_rpm, parsed, 0, NULL)) {
			d->row.peak_rpm	= INT_MIN;
			d->row.error		= true;
		}
		break;
	case DF_CITY_MPG:
		if (strtoi32_s(&d->row.city_mpg, parsed, 0, NULL)) {
			d->row.city_mpg		= INT_MIN;
			d->row.error		= true;
		}
		break;
	case DF_HIWAY_MPG:
		if (strtoi32_s(&d->row.hiway_mpg, parsed, 0, NULL)) {
			d->row.hiway_mpg	= INT_MIN;
			d->row.error		= true;
		}
		break;
	case DF_PRICE:
		if (strtoi32_s(&d->row.price, parsed, 0, NULL)) {
			d->row.price		= INT_MIN;
			d->row.error		= true;
		}
		break;
	default:
		perrorx(parsed);
	}

	if (d->row.error)
		perrorx(parsed);
	d->ncol++;
}

static
void	parse_row		(int c, void *data)
{
	struct Data	*d	= data;

	ALX_UNUSED(c);

	if (alx_llist_append(d->rows, d->nrow, &d->row, sizeof(d->row)))
		perrorx(NULL);
	memset(&d->row, 0, sizeof(d->row));
	d->ncol	= 0;
	d->nrow++;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/

