voronoi2: main.o calc.o stage_1.o polygon_read.o polygon_free.o dcel_create.o \
			dcel_free.o stage_2.o towers_read.o towers_free.o stage_3.o\
			faces_create.o faces_free.o split_polygon.o vorocells_create.o\
			vorocells_free.o stage_4.o info_free.o
	gcc -Wall -o voronoi2 main.o calc.o stage_1.o polygon_read.o polygon_free.o\
			dcel_create.o dcel_free.o stage_2.o towers_read.o towers_free.o\
			stage_3.o faces_create.o faces_free.o split_polygon.o \
			vorocells_create.o vorocells_free.o stage_4.o info_free.o -lm

main.o: main.c topo_strt.h calc.h watchtowers.h stages.h
	gcc -Wall -o main.o main.c -c

calc.o: calc.c topo_strt.h calc.h
	gcc -Wall -o calc.o calc.c -c

stage_1.o: stage_1.c calc.h topo_strt.h watchtowers.h stages.h
	gcc -Wall -o stage_1.o stage_1.c -c

polygon_read.o: polygon_read.c topo_strt.h
	gcc -Wall -o polygon_read.o polygon_read.c -c

polygon_free.o: polygon_free.c topo_strt.h
	gcc -Wall -o polygon_free.o polygon_free.c -c

dcel_create.o: dcel_create.c topo_strt.h
	gcc -Wall -o dcel_create.o dcel_create.c -c

dcel_free.o: dcel_free.c dcel_create.o topo_strt.h
	gcc -Wall -o dcel_free.o dcel_free.c -c

stage_2.o: stage_2.c polygon_read.o dcel_create.o calc.h topo_strt.h\
			watchtowers.h stages.h 
	gcc -Wall -o stage_2.o stage_2.c -c

towers_read.o: towers_read.c watchtowers.h
	gcc -Wall -o towers_read.o towers_read.c -c

towers_free.o: towers_free.c towers_read.o watchtowers.h
	gcc -Wall -o towers_free.o towers_free.c -c

faces_create.o: faces_create.c dcel_create.o topo_strt.h
	gcc -Wall -o faces_create.o faces_create.c -c

faces_free.o: faces_free.c faces_create.o topo_strt.h
	gcc -Wall -o faces_free.o faces_free.c -c

stage_3.o: stage_3.c polygon_read.o dcel_create.o towers_read.o faces_create.o\
			vorocells_create.o calc.h topo_strt.h watchtowers.h stages.h
	gcc -Wall -o stage_3.o stage_3.c -c

split_polygon.o: split_polygon.c topo_strt.h watchtowers.h
	gcc -Wall -o split_polygon.o split_polygon.c -c

vorocells_create.o: vorocells_create.c topo_strt.h watchtowers.h
	gcc -Wall -o vorocells_create.o vorocells_create.c -c 

vorocells_free.o: vorocells_free.c vorocells_create.o topo_strt.h watchtowers.h
	gcc -Wall -o vorocells_free.o vorocells_free.c -c 

stage_4.o: stage_4.c polygon_read.o dcel_create.o towers_read.o faces_create.o\
			vorocells_create.o calc.h topo_strt.h watchtowers.h stages.h
	gcc -Wall -o stage_4.o stage_4.c -c

info_free.o: info_free.c stage_3.o topo_strt.h
	gcc -Wall -o info_free.o info_free.c -c
