#include <string.h>
#include <stdlib.h>
#include <stdio.h>

float atomicRadii(char *symbol, int type)
{

        float r=0.0;
	if      (strcasecmp(symbol,"H")==0)  {if (type==1) r=0.4; else r=1.2;}
	else if (strcasecmp(symbol,"HE")==0) {if (type==1) r=0.3; else r=1.4;}
	else if (strcasecmp(symbol,"LI")==0) {if (type==1) r=1.3; else r=1.8;}
	else if (strcasecmp(symbol,"BE")==0) {if (type==1) r=0.9; else r=1.1;}
	else if (strcasecmp(symbol,"B")==0)  {if (type==1) r=0.8; else r=1.7;}
	else if (strcasecmp(symbol,"C")==0)  {if (type==1) r=0.8; else r=1.7;}
	else if (strcasecmp(symbol,"N")==0)  {if (type==1) r=0.8; else r=1.6;}
	else if (strcasecmp(symbol,"O")==0)  {if (type==1) r=0.7; else r=1.5;}
	else if (strcasecmp(symbol,"F")==0)  {if (type==1) r=0.7; else r=1.5;}
	else if (strcasecmp(symbol,"NE")==0) {if (type==1) r=0.7; else r=1.5;}
	else if (strcasecmp(symbol,"NA")==0) {if (type==1) r=1.5; else r=2.2;}
	else if (strcasecmp(symbol,"MG")==0) {if (type==1) r=1.3; else r=1.7;}
	else if (strcasecmp(symbol,"AL")==0) {if (type==1) r=1.2; else r=1.4;}
	else if (strcasecmp(symbol,"SI")==0) {if (type==1) r=1.1; else r=2.1;}
	else if (strcasecmp(symbol,"P")==0)  {if (type==1) r=1.1; else r=1.8;}
	else if (strcasecmp(symbol,"S")==0)  {if (type==1) r=1.0; else r=1.8;}
	else if (strcasecmp(symbol,"CL")==0) {if (type==1) r=1.0; else r=1.8;}
	else if (strcasecmp(symbol,"AR")==0) {if (type==1) r=1.0; else r=1.9;}
	else if (strcasecmp(symbol,"K")==0)  {if (type==1) r=2.0; else r=2.8;}
	else if (strcasecmp(symbol,"CA")==0) {if (type==1) r=1.7; else r=2.0;}
	else if (strcasecmp(symbol,"SC")==0) {if (type==1) r=1.4; else r=1.6;}
	else if (strcasecmp(symbol,"TI")==0) {if (type==1) r=1.4; else r=1.5;}
	else if (strcasecmp(symbol,"V")==0)  {if (type==1) r=1.3; else r=1.3;}
	else if (strcasecmp(symbol,"CR")==0) {if (type==1) r=1.3; else r=1.3;}
	else if (strcasecmp(symbol,"MN")==0) {if (type==1) r=1.4; else r=1.3;}
	else if (strcasecmp(symbol,"FE")==0) {if (type==1) r=1.3; else r=1.4;}
	else if (strcasecmp(symbol,"CO")==0) {if (type==1) r=1.3; else r=1.4;}
	else if (strcasecmp(symbol,"NI")==0) {if (type==1) r=1.2; else r=1.4;}
	else if (strcasecmp(symbol,"CU")==0) {if (type==1) r=1.4; else r=1.4;}
	else if (strcasecmp(symbol,"ZN")==0) {if (type==1) r=1.3; else r=1.4;}
	else if (strcasecmp(symbol,"GA")==0) {if (type==1) r=1.3; else r=1.4;}
	else if (strcasecmp(symbol,"GE")==0) {if (type==1) r=1.2; else r=1.4;}
	else if (strcasecmp(symbol,"AS")==0) {if (type==1) r=1.2; else r=1.4;}
	else if (strcasecmp(symbol,"SE")==0) {if (type==1) r=1.2; else r=1.4;}
	else if (strcasecmp(symbol,"BR")==0) {if (type==1) r=1.1; else r=1.4;}
	else if (strcasecmp(symbol,"KR")==0) {if (type==1) r=1.1; else r=1.4;}
	else if (strcasecmp(symbol,"RB")==0) {if (type==1) r=2.1; else r=1.4;}
	else if (strcasecmp(symbol,"SR")==0) {if (type==1) r=1.9; else r=1.4;}
	else if (strcasecmp(symbol,"Y")==0)  {if (type==1) r=1.6; else r=1.4;}
	else if (strcasecmp(symbol,"ZR")==0) {if (type==1) r=1.5; else r=1.4;}
	else if (strcasecmp(symbol,"NB")==0) {if (type==1) r=1.4; else r=1.4;}
	else if (strcasecmp(symbol,"AU")==0) {if (type==1) r=1.34; else r=1.79;}

	else{
		fprintf(stderr, "\n\n!!!!!  CANNOT FIND ELEMENT %s in PeriodicTable.c, Quitting..  !!!!!\n\n", symbol);
		abort();
	}

	return r;
}



