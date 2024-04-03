#include "universal_manager.h"
#include <stdlib.h>
#include <string.h>

void initUniversalManager(UniversalManager* manager) {
    if (manager) {
        manager->galaxies = NULL;
        manager->numGalaxies = 0;
        manager->companies = NULL;
        manager->numCompanies = 0;
    }
}
Galaxy* createGalaxy() {
    char name[MAX_GALAXY_NAME];
    printf("Enter Galaxy Name: ");
    scanf("%255s", name); // Get the galaxy name from the user

    Galaxy* galaxy = (Galaxy*)malloc(sizeof(Galaxy)); // Allocate memory for the new Galaxy
    if (galaxy) {
        galaxy->name = strdup(name); // Duplicate the name and assign it to the Galaxy
        // Initialize other Galaxy attributes as needed...
    }
    return galaxy; // Return the pointer to the new Galaxy
}
void addGalaxy(UniversalManager* manager, Galaxy* galaxy) {
    if (manager && galaxy) {
        Galaxy** new_galaxies_array = realloc(manager->galaxies, (manager->numGalaxies + 1) * sizeof(Galaxy*));
        if (new_galaxies_array) {
            manager->galaxies = new_galaxies_array;
            manager->galaxies[manager->numGalaxies] = galaxy;
            manager->numGalaxies++;
        }
    }
}
void addGalaxyToManager(UniversalManager* manager) {
    Galaxy* galaxy = createGalaxy(); // Create a new Galaxy
    if (galaxy) {
        addGalaxy(manager, galaxy); // Add the new Galaxy to the UniversalManager
    }
    else {
        printf("Failed to create a new Galaxy.\n");
    }
}
/*
Company* createCompany() {
    
    Company* company = (Company*)malloc(sizeof(Company));

    char name[MAX_COMPANY_NAME];
    int established, numOfSpaceCrafts, numOfTravels;
    Permission permission;
    
    printf("Enter Company Name: ");
    scanf("%255s", name); // Get the company name from the user

    do {
        printf("Enter Company's year of establishment: ");
        scanf("%d", &established);
    } while (established <= 0);


    do {
        printf("Enter Company's amount of Space Crafts: ");
        scanf("%d", &numOfSpaceCrafts);
    } while (numOfSpaceCrafts < 0);
    
    SpaceCraft* spaceCrafts = NULL;
    if (numOfSpaceCrafts > 0) {
        spaceCrafts = malloc(numOfSpaceCrafts * sizeof(SpaceCraft));
        for (int i = 0; i < numOfSpaceCrafts; i++) {
            char craftName[MAX_LEN_SPACE_CRAFT], craftModel[MAX_LEN_SPACE_CRAFT];
            double craftMaxSpeed = -1;
            int id = i + 1;

            printf("Enter name for spacecraft %d: ", id);
            scanf("%99s", craftName);

            printf("Enter model for spacecraft %d: ", id);
            scanf("%99s", craftModel);

            do {
                printf("Enter max speed (in km/s) for spacecraft %d (0 - 300000): ", i + 1);
                scanf("%lf", &craftMaxSpeed);
                if (craftMaxSpeed < MIN_SPEED || craftMaxSpeed > MAX_SPEED) {
                    printf("Invalid speed. Please enter a value between 0 and 300000.\n");
                }
            } while (craftMaxSpeed < MIN_SPEED || craftMaxSpeed > MAX_SPEED);

            char* dynamicCraftName = strdup(craftName);
            char* dynamicCraftModel = strdup(craftModel);

            SpaceCraft* newCraft = create_spacecraft(dynamicCraftName, dynamicCraftModel, craftMaxSpeed, id);

            if (!newCraft) {
                printf("Failed to create spacecraft %d.\n", i + 1);
                free(dynamicCraftName);
                free(dynamicCraftModel);
                break;
            }
            company->spaceCrafts[i] = newCraft;
        }
    }

    do {
        printf("Enter permission zone (0: No Permission, 1: Planet, 2: SolarSystem, 3: Galaxy): ");
        scanf("%d", (int*)&permission);
    } while (permission < eNOPERMISSION || permission >= eNUMOFPERMISSION);


    // Allocate memory for the new Company
    if (company) {
        company->name = strdup(name);
        company->establishedYear = established;
        company->numSpacecrafts = numOfSpaceCrafts;
        company->travels = NULL;        // implement
        company->numTravels = 0;        // implement
        company->permissionsZone = permission;
          
    }
    return company; // Return the pointer to the new Company
}
*/

void addCompany(UniversalManager* manager, Company* company, Permission permission) {
    if (manager && company) {
        company->permissionsZone = permission;
        Company** new_companies_array = realloc(manager->companies, (manager->numCompanies + 1) * sizeof(Company*));
        if (new_companies_array) {
            manager->companies = new_companies_array;
            manager->companies[manager->numCompanies] = company;
            manager->numCompanies++;
        }
    }
}
void addCompanyToManager(UniversalManager* manager) {
    
    if (manager) {

        Company* company = (Company*)malloc(sizeof(Company));
        test_createCompany(company);





    }



    /*
    Company* company = createCompany(); // Create a new Company
    if (company) {
        addCompany(manager, company, company->permissionsZone); // Add the new Company to the UniversalManager
    }
    else {
        printf("Failed to create a new Company.\n");
    }
    */
}

void printGalaxies(const UniversalManager* manager) {
    if (manager && manager->galaxies) {
        printf("List of Galaxies:\n");
        for (int i = 0; i < manager->numGalaxies; i++) {
            if (manager->galaxies[i]) {
                printf("Galaxy %d: %s\n", i + 1, manager->galaxies[i]->name);
            }
        }
    }
    else {
        printf("No galaxies are currently managed.\n");
    }
}

void printCompanies(const UniversalManager* manager) {
    if (manager && manager->companies) {
        printf("List of Companies:\n");
        for (int i = 0; i < manager->numCompanies; i++) {
            if (manager->companies[i]) {
                printf("Company %d: %s, Permission Zone: %d\n", i + 1, manager->companies[i]->name, manager->companies[i]->permissionsZone);
            }
        }
    }
    else {
        printf("No companies are currently managed.\n");
    }
}

void freeUniversalManager(UniversalManager* manager) {
    if (manager) {
        for (int i = 0; i < manager->numGalaxies; i++) {
            free_galaxy(manager->galaxies[i]); // Assumes a free_galaxy function exists
        }
        free(manager->galaxies);

        for (int i = 0; i < manager->numCompanies; i++) {
            // Assumes a free_company function exists
            free_company(manager->companies[i]);
        }
        free(manager->companies);
    }
}

void loadSystemData(UniversalManager* manager, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Assuming each line contains either "Galaxy" or "Company" followed by respective names and attributes
        char type[10], name[50];
        if (sscanf(line, "%s %s", type, name) == 2) {
            if (strcmp(type, "Galaxy") == 0) {
                Galaxy* galaxy = (Galaxy*)malloc(sizeof(Galaxy));
                galaxy->name = strdup(name); // Assuming Galaxy struct has a name field
                // Initialize other galaxy fields as needed...
                addGalaxy(manager, galaxy);
            }
            else if (strcmp(type, "Company") == 0) {
                Company* company = (Company*)malloc(sizeof(Company));
                company->name = strdup(name); // Assuming Company struct has a name field
                // Initialize other company fields and set permissions as needed...
                addCompany(manager, company, eNOPERMISSION); // Example permission
            }
        }
    }

    fclose(file);
}

void increaseRiskLevels(UniversalManager* manager) {
    if (manager && manager->galaxies) {
        for (int i = 0; i < manager->numGalaxies; i++) {
            if (manager->galaxies[i]) {
                manager->galaxies[i]->riskLevel++; // Increase the risk level of each galaxy
                printf("Increased risk level of Galaxy %s to %d\n", manager->galaxies[i]->name, manager->galaxies[i]->riskLevel);
            }
        }
    }
    else {
        printf("No galaxies to update.\n");
    }
}

Galaxy* findGalaxy(UniversalManager* manager, const char* name) {
    for (int i = 0; i < manager->numGalaxies; i++) {
        if (strcmp(manager->galaxies[i]->name, name) == 0) {
            return manager->galaxies[i];
        }
    }
    return NULL; // Not found
}

SolarSystem* findSolarSystem(Galaxy* galaxy, const char* name) {
    for (int i = 0; i < galaxy->num_solar_systems; i++) {
        if (strcmp(galaxy->star_systems[i]->name, name) == 0) {
            return galaxy->star_systems[i];
        }
    }
    return NULL; // Not found
}

Planet* findPlanet(SolarSystem* system, const char* name) {
    for (int i = 0; i < system->num_planets; i++) {
        if (strcmp(system->planets[i]->name, name) == 0) {
            return system->planets[i];
        }
    }
    return NULL; // Not found
}

void displayAllPlanets(const UniversalManager* manager) {
    // Assuming the UniversalManager has a method to iterate through all galaxies and their solar systems
    printf("Displaying all planets in the Universal Manager:\n");
    // This is a simplified placeholder. Actual iteration and display logic goes here...
    printf("Planet display functionality is not implemented yet.\n");
}

void renamePlanet(UniversalManager* manager) {
    char galaxyName[256], systemName[256], planetName[256], newName[256];
    printf("Enter Galaxy Name: ");
    scanf("%255s", galaxyName);
    printf("Enter Solar System Name: ");
    scanf("%255s", systemName);
    printf("Enter Planet Name: ");
    scanf("%255s", planetName);
    printf("Enter New Planet Name: ");
    scanf("%255s", newName);

    Galaxy* galaxy = findGalaxy(manager, galaxyName);
    if (!galaxy) {
        printf("Galaxy '%s' not found.\n", galaxyName);
        return;
    }

    SolarSystem* system = findSolarSystem(galaxy, systemName);
    if (!system) {
        printf("Solar System '%s' not found in Galaxy '%s'.\n", systemName, galaxyName);
        return;
    }

    Planet* planet = findPlanet(system, planetName);
    if (!planet) {
        printf("Planet '%s' not found in Solar System '%s'.\n", planetName, systemName);
        return;
    }

    // Rename the planet
    strncpy(planet->name, newName, sizeof(planet->name) - 1);
    planet->name[sizeof(planet->name) - 1] = '\0'; // Ensure null termination
    printf("Planet '%s' in Solar System '%s' of Galaxy '%s' renamed to '%s'.\n", planetName, systemName, galaxyName, newName);
}

void display_all_travels(const UniversalManager* manager) {
    if (!manager) {
        printf("No manager initialized.\n");
        return;
    }
    printf("\n--- All Travels ---\n");
    for (int i = 0; i < manager->numCompanies; i++) {
        Company* company = manager->companies[i];
        printf("\nCompany: %s\n", company->name);
        for (int j = 0; j < company->numTravels; j++) {
            InterstellarTravel* travel = company->travels[j];
            printf("Travel %d: From %s to %s\n", j + 1, travel->travelCodeSrc, travel->travelCodeDst);
            printf("  Spacecraft: %s (ID: %s, Model: %s, Max Speed: %.2f)\n",travel->spaceCraft->name, travel->spaceCraft->craftId ,travel->spaceCraft->model, travel->spaceCraft->maxSpeed);
            
            printf("  Departure: ");
            print_date(&travel->departureDate);
            
            printf(", Arrival: ");
            print_date(&travel->arrivalDate);
            printf(", Distance: %.2f, Risk Level: %d, Completed: %s\n", travel->distance, travel->riskLevel, travel->isCompleted ? "Yes" : "No");
        }
    }
}

void display_company_basic_data(Company* company){
    printf("\n--- Company Basic Data ---\n");
    printf("Name: %s\n", company->name);
    printf("Number of Spacecrafts: %d\n", company->numSpacecrafts);
    printf("Number of Travels: %d\n", company->numTravels);
    printf("Permissions Zone: %d\n", company->permissionsZone);
}
void display_company_spacecrafts(Company* company){
    printf("\n--- Company's Spacecrafts Fleet ---\n");
    for (int i = 0; i < company->numSpacecrafts; i++) {
        SpaceCraft* craft = &(company->spaceCrafts[i]);
        printf("Spacecraft %d: Name: %s, Model: %s, Max Speed: %.2f, ID: %d\n", i + 1, craft->name, craft->model, craft->maxSpeed, craft->craftId);
    }
}
void display_company_travels(Company* company){
    printf("\n--- Company's Incoming Travels ---\n");
    for (int i = 0; i < company->numTravels; i++) {
        InterstellarTravel* travel = company->travels[i];
        printf("Travel %d: Src: %s, Dst: %s, SpaceCraft: %s, Departure: ", i + 1, travel->travelCodeSrc, travel->travelCodeDst, travel->spaceCraft->name);
        print_date(&travel->departureDate);
        printf(", Arrival: ");
        print_date(&travel->arrivalDate);
        printf(", Distance: %.2f, Risk Level: %d\n", travel->distance, travel->riskLevel);
    }
}

void add_spacecraft_to_company(Company* company, UniversalManager* manager){
    
    if (company->numSpacecrafts >= MAX_AMOUNT_OF_CRAFTS) {
        printf("Maximum number of spacecrafts reached.\n");
        return;
    }

    char* name = (char*)malloc(MAX_LEN_SPACE_CRAFT * sizeof(char));     // MAX = 100
    char* model = (char*)malloc(MAX_LEN_SPACE_CRAFT * sizeof(char));    // MAX = 100
    double maxSpeed;
    int craftId = company->numSpacecrafts + 1;  // Example ID generation logic
     
    printf("Enter spacecraft name: ");                  // Name checker
    scanf("%99s", name);
    printf("Enter spacecraft model: ");                 // Model checker
    scanf("%99s", model);
    printf("Enter max speed Max speed is: 300,000: ");  // Speed checker
    scanf("%lf", &maxSpeed);

    // Ensure the company's spaceCrafts array is large enough to hold another spacecraft
    company->spaceCrafts = realloc(company->spaceCrafts, (company->numSpacecrafts + 1) * sizeof(SpaceCraft));

    // Allocate a new spacecraft in the array
    SpaceCraft* newCraft = &company->spaceCrafts[company->numSpacecrafts];
    newCraft->name = name;  // name is already dynamically allocated
    newCraft->model = model;  // model is already dynamically allocated
    newCraft->maxSpeed = maxSpeed;
    newCraft->craftId = craftId;

    company->numSpacecrafts++;

    printf("Spacecraft '%s' added to company '%s'.\n", newCraft->name, company->name);

    free(name);
    free(model);

}

void add_travel_to_company(Company* company, UniversalManager* manager){}
void edit_company_permissions(Company* company, UniversalManager* manager){}
void export_company_data_to_bin(Company* company, const char* filename){}
void export_company_data_to_txt(Company* company, const char* filename){}

void manage_company_operations(UniversalManager* manager) {
    if (!manager) {
        printf("Universal Manager is not initialized.\n");
        return;
    }

    int choice;
    do {
        printf("\nCompany Management Menu:\n");
        printf("1. Show all Companies\n");
        printf("2. Show all Travels\n");
        printf("3. Manage Specific Company\n");
        printf("4. Export all Data\n");
        printf("5. Return to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printCompanies(manager);
            break;
        case 2:
            display_all_travels(manager);
            break;
        case 3: {
            char companyName[MAX_COMPANY_NAME];     // Max is 256 chars
            printf("Enter Company Name: ");
            scanf("%255s", companyName);  // Read the company name

            Company* selectedCompany = NULL;
            for (int i = 0; i < manager->numCompanies; i++) {
                if (strcmp(manager->companies[i]->name, companyName) == 0) {
                    selectedCompany = manager->companies[i];
                    break;  // Company found, exit the loop
                }
            }

            if (selectedCompany) {
                manage_specific_company(manager, selectedCompany);
            }
            else {
                printf("Company with name '%s' not found.\n", companyName);
            }
            break;
        }

        case 4:
            
            /*
            Export all the companies data:
                1. Export to Bin file
                2. Export to txt file
            */

            break;
        case 5:
            printf("Returning to Main Menu...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

void manage_specific_company(UniversalManager* manager, Company* company) {
    if (!company) {
        printf("Company not found.\n");
        return;
    }

    int choice;
    do {
        printf("\nManaging Company: %s\n", company->name);
        printf("1. Display Basic Data\n");
        printf("2. Display company's space crafts fleet\n");
        printf("3. Display incomming Travels\n");
        printf("4. Add Space craft\n");
        printf("5. Add Travel\n");
        printf("6. Edit Permissions\n");
        printf("7. Return to Company Management Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            // Display Basic Data (Company's name, ID, number of travels, number of space crafts, Flight Permissions)
            display_company_basic_data(company);
            break;
        case 2:
            // Display company's space crafts fleet (display for each space craft: name, model, maxSpeed, craftID)
            display_company_spacecrafts(company);
            break;
        case 3:
            // Display incomming Travels (display for each travel: travelCodeSrc,travelCodeDst, spaceCraft, departureDate, arrivalDate, distance, riskLevel)
            display_company_travels(company);
            break;
        case 4:
            // Add Space craft (add a new space craft from universalManager or create a new one and update also in universalManager)
            add_spacecraft_to_company(company, manager);
            break;
        case 5:
            // Add Travel (create a new travel based on the company' limitation: Permission zone and space crafts) 
            add_travel_to_company(company, manager);
            break;
        case 6:
            // Edit Permissions (downgrade or update permissions, needs to send a request to universalManager - and get an approval response from universalManager)
            edit_company_permissions(company, manager);
            break;
        case 7:
            printf("Returning to Company Management Menu...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
}

