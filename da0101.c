/******************************************************************************
 *	Copyright (C) 2020	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	BSD-2-Clause			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <csv.h>

#define ALX_NO_PREFIX
#include <libalx/base/errno/error.h>
#include <libalx/base/libgen/basename.h>
#include <libalx/base/stdio/printf/sbprintf.h>
#include <libalx/base/stdio/printf/snprintfs.h>


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/

/******************************************************************************
 ******* enum / struct / union ************************************************
 ******************************************************************************/
struct	Data_Frame {
	int	symboling;
	int	norm_losses;
	int	make;
	int	fuel;
	bool	aspiration;
	int	doors;
	int	style;
	int	drive_wh;
	int	engine_pos;
	double	wheel_base;
	double	length;
	double	width;
	double	height;
	int	curb_weight;
	int	engine_type;
	int	cylinders;
	int	engine_size;
	int	fuel_system;
	double	bore;
	double	stroke;
	double	compression_ratio;
	int	hp;
	int	peak_rpm;
	int	city_mpg;
	int	hiway_mpg;
	int	price;
};

struct	Data {
	struct Alx_LinkedList	*llist;
	struct Data_Frame	*row;
};


/******************************************************************************
 ******* static variables *****************************************************
 ******************************************************************************/
static	const char	*url	= "https://s3-api.us-geo.objectstorage.softlayer.net/cf-courses-data/CognitiveClass/DA0101EN/auto.csv";
static	const char	*headers[]	= [
	"symboling", "normalized-losses", "make", "fuel-type", "aspiration",
	"num-of-doors", "body-style", "drive-wheels", "engine-location",
	"wheel-base", "length", "width", "height", "curb-weight",
	"engine-type", "num-of-cylinders", "engine-size", "fuel-system",
	"bore", "stroke", "compression-ratio", "horsepower", "peak-rpm",
	"city-mpg", "highway-mpg", "price"];
static	ptrdiff_t	nrow;
static	ptrdiff_t	ncol;


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
void	parse_field	(void *parsed, size_t size, void *data);
static
void	parse_row	(int c, void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
int	main	(void)
{
	char			file[FILENAME_MAX];
	struct Alx_LinkedList	*llist;
	struct csv_parser	parser;
	FILE			*fp;
	size_t			n;
	char			buf[BUFSIZ];
	struct Data_Frame	row;
	int			status;
	struct Data		data;

	status	= 0;

	wget(url);
	get_fname(file, url);

	if (llist_init(&llist))
		errorx(EXIT_FAILURE, "llist_init()");
	data.llist	= llist;
	data.row	= &row;

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

	do {
		n	= fread(buf, 1, ARRAY_SIZE(buf), fp);
		if (csv_parse(&parser, buf, n, &, &, &data) < n) {
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
	alx_llist_deinit(llist);

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

static
void	prn_data	(const char *fname)
{
	char	cmd[_POSIX_ARG_MAX];

	printf("
	for (ptrdiff_t i = 1; i < FIELDS; i++) {
		printf("



	if (sbprintf(cmd, NULL, "less %s", fname))
		errorx(EXIT_FAILURE, fname);
	if (system(cmd))
		errorx(EXIT_FAILURE, cmd);
}

static
void	parse_field	(void *parsed, size_t size, void *data)
{

	switch (row) {
	case:
	default:
	}

	row++;
}

static
void	parse_row	(int c, void *data)
{

	row = 0;
	col++;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/

