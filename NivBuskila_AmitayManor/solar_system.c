#include "solar_system.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void debugCompareSS(const SolarSystem* pSS1, const SolarSystem* pSS2) {
    printf("Comparing Planets...\n");
    printf("Name: %s - %s\n", pSS1->name, pSS2->name);
    printf("Location X: %d - %d\n", pSS1->portal_location.x, pSS2->portal_location.x);
    printf("Location Y: %d - %d\n", pSS1->portal_location.y, pSS2->portal_location.y);
    printf("Location Z: %d - %d\n", pSS1->portal_location.z, pSS2->portal_location.z);
    printf("ID: %d - %d\n", pSS1->id, pSS2->id);
    printf("Risk Level: %d - %d\n", pSS1->risk_level, pSS2->risk_level);
    printf("Size: %d - %d\n", pSS1->size, pSS2->size);
    printf("\n\n");
    debugComparePlanets(pSS1->planetsHead, pSS2->planetsHead);
}

void testPlanetAndSolarSystemReadWrite() {
    // Create a sample planet and solar system
    Planet earth = { "Earth", {1, 2, 3}, 45, 10, 1000 };
    SolarSystem sol = { "Sol", {100, 200, 300}, 2, 9000, 1, 1 };

    PlanetNode earthNode = { &earth, NULL };
    sol.planetsHead = &earthNode;

    // Write to text file
    FILE* fp = fopen("test_solarsystem.txt", "w");
    writeSolarSystemToText(fp, &sol);
    fclose(fp);

    // Read back
    SolarSystem solRead;
    fp = fopen("test_solarsystem.txt", "r");
    readSolarSystemFromText(fp, &solRead);
    fclose(fp);

    debugCompareSS(&sol, &solRead);
    // Implement comparison functions and assert results
    //assert(comparePlanets(&earth, solRead.planetsHead->planet));
    assert(compareSolarSystems(&sol, &solRead));


    // Free dynamically allocated memory
    // Free planet nodes in solRead
}

int compareSolarSystems(const SolarSystem* pSS1, const SolarSystem* pSS2) {
    if (strcmp(pSS1->name, pSS2->name) != 0) return 0;
    if (pSS1->portal_location.x != pSS2->portal_location.x ||
        pSS1->portal_location.y != pSS2->portal_location.y ||
        pSS1->portal_location.z != pSS2->portal_location.z) return 0;
    if (pSS1->id != pSS2->id) return 0;
    if (pSS1->risk_level != pSS2->risk_level) return 0;
    if (pSS1->size != pSS2->size) return 0;
    if (!comparePlanets(pSS1->planetsHead, pSS2->planetsHead)) return 0;
    return 1;
}

void readSolarSystemFromText(FILE* fp, SolarSystem* system) {
    char buffer[256]; // Temporary buffer for reading lines
    fgets(buffer, sizeof(buffer), fp); // Read the solar system name line
    sscanf(buffer, "SolarSystem Name: %[^\n]", system->name);

    fgets(buffer, sizeof(buffer), fp); // Read the portal location line
    sscanf(buffer, "Portal Location: %d %d %d", &system->portal_location.x, &system->portal_location.y, &system->portal_location.z);

    fgets(buffer, sizeof(buffer), fp); // Read the risk level line
    sscanf(buffer, "Risk Level: %d", &system->risk_level);

    fgets(buffer, sizeof(buffer), fp); // Read the size line
    sscanf(buffer, "Size: %d", &system->size);

    fgets(buffer, sizeof(buffer), fp); // Read the ID line
    sscanf(buffer, "ID: %d", &system->id);

    fgets(buffer, sizeof(buffer), fp); // Read the number of planets line
    int numPlanets;
    sscanf(buffer, "Number of Planets: %d", &numPlanets);

    system->planetsHead = NULL;
    PlanetNode* lastNode = NULL;

    for (int i = 0; i < numPlanets; ++i) {
        PlanetNode* newNode = (PlanetNode*)malloc(sizeof(PlanetNode));
        newNode->planet = (Planet*)malloc(sizeof(Planet));
        newNode->next = NULL;

        readPlanetFromText(fp, newNode->planet);

        if (system->planetsHead == NULL) {
            system->planetsHead = newNode;
        }
        else {
            lastNode->next = newNode;
        }
        lastNode = newNode;

        fgets(buffer, sizeof(buffer), fp); // Read the delimiter line for the next planet
        // Check for the end of the solar system entry
        if (strstr(buffer, "===") != NULL) {
            break;
        }
    }
}

void writeSolarSystemToText(FILE* fp, const SolarSystem* system) {
    fprintf(fp, "SolarSystem Name: %s\n", system->name);
    fprintf(fp, "Portal Location: %d %d %d\n", system->portal_location.x, system->portal_location.y, system->portal_location.z);
    fprintf(fp, "Risk Level: %d\n", system->risk_level);
    fprintf(fp, "Size: %d\n", system->size);
    fprintf(fp, "ID: %d\n", system->id);
    fprintf(fp, "Number of Planets: %d\n", system->num_planets);

    PlanetNode* current = system->planetsHead;
    while (current != NULL) {
        writePlanetToText(fp, current->planet);
        current = current->next;
    }
    fprintf(fp, "===\n");
}


void addPlanet(SolarSystem* system, Planet* planet) {
    PlanetNode* newNode = ALLOCATE(PlanetNode*, 1);
    if (newNode == NULL) {
        printf("Failed to allocate memory for the new planet node.\n");
        return;
    }

    
    newNode->planet = planet;
    newNode->next = system->planetsHead;
    system->planetsHead = newNode;
    system->num_planets++;

}

void removePlanet(SolarSystem* system, int planetID) {
   
    PlanetNode* current = system->planetsHead;
    PlanetNode* prev = NULL;

    while (current != NULL) {
        Planet* p = (Planet*)current->planet;
        if (p->id == planetID) {
            if (prev == NULL) {
                system->planetsHead = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current);
            system->num_planets--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void freePlanetList(SolarSystem* system) {
    
    PlanetNode* current = system->planetsHead;
    while (current != NULL) {
        PlanetNode* temp = current;
        current = current->next;
        free(temp);
    }
    system->planetsHead = NULL;  // Ensure the head is NULL after freeing
    system->num_planets = 0;
}



void print_solar_system(void* ss) {
    SolarSystem* system = (SolarSystem*)ss;
    printf("Solar System ID: %d, Name: %s, Number of Planets: %d\n", system->id, system->name, system->num_planets);
    
    PlanetNode* current = system->planetsHead; // Start with the head of the linked list
    while (current != NULL) {

        generic_print((Planet*)current->planet, print_planet); 
        current = current->next; 
    }
}

SolarSystem* create_solar_system(Galaxy* galaxy) {
    SolarSystem* system = ALLOCATE(SolarSystem*, 1);
//    SolarSystem* system = malloc(sizeof(SolarSystem));
    if (system) {
       
        printf("Enter Solar System Name: ");
        if (!myGets(system->name, MAX_SOLAR_SYSTEM_NAME)) {
            LOG_DEBUG("Failed to read Solar System name.\n");
//            fprintf(stderr, "Failed to read Solar System name.\n");
            free(system);
            return NULL;
        }
        system->risk_level = 0;
        system->num_planets = 0;
        system->planetsHead = NULL; 
        system->num_planets = 0;

        int idFlag = 0;
        int id;
        do {
            printf("Enter the Solar System ID (1-9999): ");
            scanf("%d", &id);
            if (check_unique_solarSystem_id(galaxy, id) && id > 0 && id < 10000) {
                idFlag = 1;
                system->id = id;
            }
            else
                printf("\nError! ID is not valid. Try again.\n");

        } while (!idFlag);

        int locFlag = 0;
        do {
            printf("Enter the Solar System location coordinates (x y z): ");
            scanf("%d %d %d", &system->portal_location.x, &system->portal_location.y, &system->portal_location.z);

            if (check_unique_solarSystem_location(galaxy, system->portal_location))
                locFlag = 1;
            else
                printf("\nError! Location is not valid. Try again.\n");

        } while (!locFlag);

        int radiusFalg = 0;
        do {
            printf("Enter the Solar System radius: ");
            scanf("%d", &system->size);
            if (system->size > 0 )//&& isSolarSystemWithinGalaxy(galaxy, system))
                radiusFalg = 1;
            else
                printf("\nError! Radius is not valid. Try again.\n");
        } while (!radiusFalg);

    }
    else {
        LOG_DEBUG("Memory allocation failed for SolarSystem.\n");
//        fprintf(stderr, "Memory allocation failed for SolarSystem.\n");
    }
    return system;
}

int isPlanetWithinSolarSystem(SolarSystem* solarSystem , Planet* planet) {

    double distance = calculateDistance(planet->portal_location, solarSystem->portal_location);

    if (distance <= solarSystem->size) return 1;  
   
    else return 0;  
    
}

void display_solar_system(const SolarSystem* system) {
    if (system) {
        printf("Solar System Name: %s\n", system->name);
        printf("Solar System ID: %d\n", system->id);
        printf("Number of Planets: %d\n", system->num_planets);
    }
}

void rename_solarSystem(SolarSystem* system) {
    if (!system) {
        printf("Invalid input.\n");
        return;
    }

    char newName[MAX_SOLAR_SYSTEM_NAME];
    printf("Enter new name for the Solar System: ");
    if (myGets(newName, MAX_SOLAR_SYSTEM_NAME)) {
        strncpy(system->name, newName, MAX_SOLAR_SYSTEM_NAME - 1);
        system->name[MAX_SOLAR_SYSTEM_NAME - 1] = '\0';
        printf("Solar System successfully renamed to %s.\n", system->name);
    }
    else {
        printf("Failed to read new name.\n");
    }
}

int check_unique_planet_id(SolarSystem* sSystem, int id) {
    if (!sSystem || !sSystem->planetsHead || sSystem->num_planets <= 0)
        return 1;

    PlanetNode* current = sSystem->planetsHead;
    while (current != NULL) {
        Planet* planet = (Planet*)current->planet;
        if (planet->id == id) {
            return 0;  
        }
        current = current->next; 
    }
    return 1;
}

int check_unique_planet_location(SolarSystem* sSystem, Location planetLoc) {
    if (!sSystem || !sSystem->planetsHead || sSystem->num_planets <= 0)
        return 1;

    PlanetNode* current = sSystem->planetsHead;
    while (current != NULL) {
        Planet* planet = (Planet*)current->planet;
        if (isSameLocation(planet->portal_location, planetLoc)) {
            return 0;  
        }
        current = current->next;  
    }
    return 1;
}

void free_solar_system(SolarSystem* system) {
    if (system) {
       
        freePlanetList(system);
        free(system);
    }
}
