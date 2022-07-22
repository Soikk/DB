#include "db.h"
#include <time.h>


int main(){

	printf("%016llu\n", (uint64_t) crc64(0, (unsigned char*)"cacadevaca", 10));

	inputBuffer *in = newInputBuffer();
	
	char *str = "grandmother;football;capital;concerned;entire;realize;garden;refused;proud;tune;rhyme;other;writer;command;fresh;fence;rapidly;active;cover;repeat;determine;yard;cannot;animal;pure;rich;mirror;frozen;vast;coach;brass;activity;bottom;airplane;local;tone;attack;though;between;value;collect;mission;tower;brought;original;history;reason;minute;would;hung;strange;children;offer;blue;wrapped;magnet;color;cage;easily;percent;lower;verb;hundred;larger;away;was;certain;western;yes;lack;wish;same;spend;arrive;fog;heard;bill;effort;steam;wolf;indicate;suppose;because;life;down;seat;age;earn;under;cell;floating;although;spent;folks;swing;hello;cent;swung;pen;happened;slip;pupil;smell;fix;piano;closer;idea;trunk;model;school;particularly;he;coast;describe;such;join;been;hard;three;around;tube;soldier;baby;mouse;note;sort;house;gasoline;organized;eat;sat;crowd;alive;spoken;wide;square;luck;tales;angry;having;wear;frog;outer;nice;regular;year;clothing;check;throughout;farmer;dug;dark;exercise;table;your;form;should;personal;use;road;bright;walk;fairly;affect;but;night;close;job;front;fight;beside;ocean;herd;pass;hardly;widely;prepare;nails;paid;lucky;design;grandfather;aid;heavy;truck;sleep;difficult;log;keep;government;headed;mother;sad;bread;voyage;when;happy;making;whistle;plural;guard;therefore;continent;roof;money;pan;unusual;region;special;generally;plate;visit;look;lost;sick;wonderful;farther;put;characteristic;gravity;trap;system;twice;taste;knew;mad;smallest;automobile;return;huge;underline;danger;news;electric;information;breeze;thread;equally;five;new;average;former;wild;spend;cabin;recognize;nearest;circle;such;found;pass;whistle;slave;event;knowledge;fear;friend;am;browserling;cry;length;thy;create;busy;office;earth;blind;smallest;birthday;putting;classroom;pen;southern;summer;put;open;solution;spread;equator;else;kitchen;determine;strong;change;world;pocket;claws;earn;excellent;drove;donkey;rush;band;energy;fighting;hurt;ordinary;native;visitor;give;storm;pressure;imagine;street;engine;worth;hospital;attached;subject;perhaps;hospital;living;waste;dark;natural;change;enter;girl;motor;element;experiment;physical;value;excited;fort;layers;buy;minerals;satisfied;next;spirit;unhappy;storm;angry;science;desk;develop;behind;afraid;act;else;prepare;given;raw;affect;husband;ring;older;brought;book;cow;lake;sides;ago;fill;successful;real;aside;taught;mind;straight;date;very;chart;slabs;thin;saddle;full;sort;heard;surprise;fox;cool;dish;alphabet;early;spring;nest;sometime;date;light;break;lion;difference;rhyme;might;step;teach;potatoes;young;nine;liquid;how;lunch;heavy;mass;being;save;cutting;negative;swimming;cutting;journey;army;none;worry;leave;explore;baseball;fight;road;exact;hay;voyage;sheet;test;right;examine;agree;heart;pig;cannot;tool;hill;changing;bee;find;together;lay;tie;lost;continued;then;came;rhyme;mirror;town;substance;both;up;quite;push;shake;solid;result;you;ought;chicken;waste;freedom;why;somehow;not;complete;sick;struggle;military;pure;top;south;step;education;could;between;familiar;recognize;rich;tool;material;were;chicken;stopped;stay;policeman;round;firm";


	database *db = newDatabase("miDB");
	
	addFileTag(db, "vaca.png", "naturaleza");	
	addFileTags(db, "donald-tromp.jpg", 3, "based", "hitler", "very cool");
	
	storeDatabase(db, "db.db");
	
	db = loadDatabase("db.db");

	printDatabase(db);
	
	debugDatabase(db);


	while(0){

		prompt();
		getInput(in);
		
		/*
		insertTag(r, in->buffer);
		printf("Tags of row '%s': %s\n", r->path, r->tags);
		printf("Number of tags: %u. Length of tags: %u\n", r->numTags, r->lenTags);
		*/

		/*switch(handleInput(in)){
			case META_COMMAND_SUCCESS:
				printf("we done it nigger\n");
				break;
			case META_COMMAND_FAIL:
				printf("uh-oh nigga i dunno what '%s' is!\n", in->buffer);
				break;
		}*/
	}

	return 0;
}
