#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sp21_cop3502_as1.h"
#include "leak_detector_c.h"

///prototypes
void remove_crlf(char *s);
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length);
void read_monster_list(FILE *ifp, monster *m);
void get_region_list(FILE *ifp, region *r, int nregions, monster *m, int nmonsters);
void read_region_list(region *r , FILE *ifp, int tot_mons, monster *m);
void get_trainer_list(FILE *ifp, trainer *t, region *r, int nregions,int  ntrainers);
void read_trainer_list(FILE *ifp, trainer *t, region *a, int num_regions, int ntrainers);
void create_file(FILE *ofp, trainer *t);
int calculate_things(trainer *t, int itineraries, int nmonsters);
void free_memory(FILE *ifp, FILE *ofp,monster *m, region *r, trainer *t, int nmonsters, int nregions, int ntrainers);
void free_monster(monster *m);
void free_region(region *r);
void free_trainer_it(trainer *t);

///These functions are from Dr. Gerber

int get_next_nonblank_line(FILE *ifp, char *buf, int max_length)
{
    buf[0] = '\0';

    while(!feof(ifp) && (buf[0] == '\0'))
    {
        fgets(buf, max_length, ifp);
        remove_crlf(buf);
    }

    if(buf[0] != '\0') {
        return 1;
    } else {
        return 0;
    }
}

void remove_crlf(char *s)
{
    char *t = s + strlen(s);

    t--;

    while((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0';  // ...clobber it by overwriting it with null, and...
        t--;        // decrement t *itself (i.e., change the pointed location)
    }
}

///Function that nmonsters from first line of text file
int get_num_of_monsters(FILE *ifp)
{
    char buff [256];
    int nmonsters;

    get_next_nonblank_line(ifp, buff, 255);
    sscanf(buff, "%d", &nmonsters);

    return nmonsters;
}

///monster memory allocation for nmonsters amount of monsters
monster *mom_mem (int nmonsters)
{
  return malloc(sizeof(monster) * nmonsters);
}

///creating monster array for nmonsters amount of monsters
void get_monster_list(FILE *ifp, monster *m, int nmonsters)
{
  for (int i = 0; i < nmonsters; ++i)
    {
        read_monster_list(ifp, m + i);
    }
}
///creates the monster struct
void read_monster_list(FILE *ifp, monster *m)
{
    char buf[256];
    char m_name[256];
    char m_element[256];
    int population;

    get_next_nonblank_line(ifp, buf, 255);
    sscanf(buf, "%s %s %d", m_name, m_element, &population);

    m->name = strdup(m_name);
    m->element = strdup(m_element);
    m->population = population;
}
void print_monster_list(monster *m,int nmonsters)
{
    for (int i = 0; i < nmonsters; ++i)
    {
        printf("Monster: %s\n", m[i].name);
        printf("Element: %s\n", m[i].element);
        printf("Population: %d\n\n", m[i].population);
    }
}

void add_new_mon(monster *m, char *mname, char *melement, int pop, int nmonsters)
{

    //m[nmonsters].element = strdup(melement);
    //m[nmonsters].population = pop;
}

///gets the number of regions
int get_nregions(FILE *ifp)
{
    char buff [256];
    int nregions;

    get_next_nonblank_line(ifp, buff, 255);
    sscanf(buff, "%d", &nregions);

    return nregions;
}

///region memory allocation for nregions amount of regigons
region *region_mem(int nregions)
{
  return malloc(sizeof(region) * nregions);
}

///creating region array for nregions amount of regions
void get_region_list(FILE *ifp, region *r, int nregions, monster *m, int nmonsters)
{
    for(int i = 0; i < nregions; ++i)
    {
        read_region_list(r + i, ifp, nmonsters, m);
    }
}

///creates the region struct
void read_region_list(region *r , FILE *ifp, int tot_mons, monster *m)
{
    char buff [256];
    char rname[256];
    char r_mname[256];
    int r_nmonsters;

    r->total_population = 0;

    get_next_nonblank_line(ifp, buff, 255);
    sscanf(buff, "%s", rname);
    r->name = strdup(rname);

    get_next_nonblank_line(ifp, buff, 255);
    sscanf(buff, "%d", &r_nmonsters);
    r->nmonsters = r_nmonsters;

    r->monsters = malloc(sizeof(monster*) * r_nmonsters);

    for(int i = 0; i < r_nmonsters; ++i)
    {
        get_next_nonblank_line(ifp, buff, 255);
        sscanf(buff, "%s", r_mname);
        for ( int k = 0; k < tot_mons; ++k)
        {
            if (strcmp(r_mname, m[k].name) == 0)
            {
                r->monsters[i] = &m[k];
                r->total_population += m[k].population;
                break;
            }
        }

    }
}

///gets number of trainers
int get_ntrainers(FILE *ifp)
{
    char buff [256];
    int ntrainers;

    get_next_nonblank_line(ifp, buff, 255);
    sscanf(buff, "%d", &ntrainers);

    return ntrainers;
}

///Trainer memory allocation for ntrainers amount of trainers
trainer *trainer_mem(int ntrainers)
{
  return malloc(sizeof(trainer) * ntrainers);
}

///creating trainer array for ntrainer amount of trainers
void get_trainer_list(FILE *ifp, trainer *t, region *r, int nregions,int  ntrainers)
{
  for(int i = 0; i < nregions; ++i)
    {
        read_trainer_list(ifp, t + i, r, nregions , ntrainers);
    }
}

///creates trainer struct while allocating memory for trainer visits* and itinerary regions*
void read_trainer_list(FILE *ifp, trainer *t, region *a, int num_regions, int ntrainers)
{
    char buff [256];
    char tname[256];
    char rname[256];
    int captures;
    int nregions;

    get_next_nonblank_line(ifp, buff, 255);
    sscanf(buff, "%s", tname);
    t->name = strdup(tname);

    t->visits = malloc(sizeof(itinerary) * ntrainers);
    itinerary *it = t->visits;

    get_next_nonblank_line(ifp, buff, 255);
    sscanf(buff, "%d", &captures);
    it->captures = captures;

    get_next_nonblank_line(ifp, buff, 255);
    sscanf(buff, "%d", &nregions);
    it->nregions = nregions;

    it->regions = malloc(sizeof(region*) * nregions);

    for(int i = 0; i < nregions; ++i)
    {
        get_next_nonblank_line(ifp, buff, 255);
        sscanf(buff, "%s", rname);
        for(int j = 0; j < num_regions; ++j)
        {
            if (strcmp(rname, a[j].name) == 0)
            {
                it->regions[i] = &a[j];
                break;
            }
        }
    }
}

///iterating through trainer struct to access data to use in the create_file function
void print_output_file(FILE *ofp, trainer *t, int ntrainers)
{
   for(int i = 0; i < ntrainers; ++i)
    {
        create_file(ofp, t + i);
    }
}

///accesses data from all structs to create the proper output file
void create_file(FILE *ofp, trainer *t)
{
        fprintf(ofp,"%s\n", t->name);

        itinerary *it = t->visits;
        for (int i = 0; i < it->nregions; ++i)
        {
            region *r = it->regions[i];
            fprintf(ofp,"%s\n", r->name);
            for (int j = 0; j < r->nmonsters; ++j)
            {
                monster *m = r->monsters[j];
                int calc_captured;
                calc_captured = calculate_things(t,i,j);

                if (calc_captured > 0)
                {
                    fprintf(ofp,"%d %s\n", calc_captured, m->name);
                }
            }
        }
        fprintf(ofp,"\n");
}

///function that calculates the expected capture of each monster by each trainer
int calculate_things(trainer *t, int itineraries, int nmonsters)
{
    int captures;
    itinerary *it = t->visits;
    region *r = it->regions[itineraries];
    monster *m = r->monsters[nmonsters];

    captures = round(((float)m->population / (float)r->total_population) * it->captures);

    return captures;
}

///function that handles all the free() calling
void free_memory(FILE *ifp, FILE *ofp,monster *m, region *r, trainer *t, int nmonsters, int nregions, int ntrainers)
{
    for(int i = 0; i < nmonsters; ++i)
      {
          free_monster(m + i);
      }

      for(int i = 0; i < nregions; ++i)
      {
          free_region(r + i);
      }

      for(int i = 0; i < ntrainers; ++i)
      {
          free_trainer_it(t + i);
      }
      free(m);
      free(r);
      free(t);
      fclose(ifp);
      fclose(ofp);
}

///monster freeing
void free_monster(monster *m)
{
    free(m->name);
    free(m->element);
}

///region freeing
void free_region(region *r)
{
    free(r->name);
    free(r->monsters);
}

///trainer and itinerary freeing
void free_trainer_it(trainer *t)
{
    itinerary *it = t->visits;
    free(it->regions);
    free(t->name);
    free(t->visits);
}


int main()
{
    atexit(report_mem_leak);

    FILE *ifp = fopen("cop3502-as1-input.txt.", "r");
    //FILE *ofp = fopen("cop3502-as1-output-kostyun-jose.txt", "w");

    int nmonsters = get_num_of_monsters(ifp);
    monster *monster_dex = mom_mem(nmonsters);
    get_monster_list(ifp, monster_dex, nmonsters);


    nmonsters += 1;
    add_new_mon(monster_dex, "Port", "grass", 30, nmonsters);

    print_monster_list(monster_dex,nmonsters);

//    int index_count = 0;
//    for (int i = 0; i < nmonsters; ++i)
//    {
//        index_count += 1;
//
//
//        printf("Monster: %s\n", monster_dex[i].name);
//        printf("Element: %s\n", monster_dex[i].element);
//        printf("Population: %d\n\n", monster_dex[i].population);
//    }

    //printf("There are %d elements", index_count);


    //printf("New Monster %s\n", monster_dex[nmonsters].name);



//    int nregions = get_nregions(ifp);
//    region *atlas = region_mem(nregions);
//    get_region_list(ifp, atlas, nregions, monster_dex, nmonsters);
//
//    int ntrainers = get_ntrainers(ifp);
//    trainer *trainers = trainer_mem(ntrainers);
//    get_trainer_list(ifp, trainers, atlas, nregions, ntrainers);
//
//    print_output_file(ofp, trainers, ntrainers);
//
//    free_memory(ifp, ofp, monster_dex, atlas, trainers, nmonsters, nregions,ntrainers);

    return 0;
}
