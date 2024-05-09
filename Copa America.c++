#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>  // Necesario para std::sort
#include <fstream>

using namespace std;

struct Team;  

struct Match {
    Team* teamA;
    Team* teamB;
};

struct Player {
    string name;
    float goalProbability;
    float yellowCardProbability;
    float redCardProbability;
    int yellowCards;
    bool isSuspended;
    bool operator==(const Player& other) const {
        return name == other.name;
    }
};

struct Team {
    string name;
    vector<Player> players;
    int points = 0;
    float winProbability;
};

struct Group {
    string name;
    vector<Team> teams;
};

bool simulateEvent(float probability) {
    return ((float)rand() / RAND_MAX) < probability;
}

int simulatePenalties(Team& teamA, Team& teamB) {
    int scoreA = 0;
    int scoreB = 0;

    // Simular 5 penaltis para cada equipo
    for (int i = 0; i < 5; ++i) {
        if (simulateEvent(0.75)) {  // Supongamos que la probabilidad de marcar un penalti es del 75%
            scoreA++;
        }
        if (simulateEvent(0.75)) {
            scoreB++;
        }
    }

    // Si hay empate después de 5 penaltis, seguir con penaltis "súbitos"
    while (scoreA == scoreB) {
        if (simulateEvent(0.75)) {
            scoreA++;
        }
        if (simulateEvent(0.75)) {
            scoreB++;
        }
    }
   
    // Imprimir el resultado de los penaltis
    cout << "Resultado de los penaltis:\n";
    cout << teamA.name << " " << scoreA << " - " << scoreB << " " << teamB.name << "\n\n";


    // Devolver el resultado de los penaltis
    return scoreA > scoreB ? 1 : -1;
}

void simulateMatch(Team& teamA, Team& teamB) {
    int scoreA = 0;
    int scoreB = 0;

    vector<string> goalsTeamA;
    vector<string> goalsTeamB;
    vector<string> yellowCardsTeamA;
    vector<string> yellowCardsTeamB;
    vector<string> redCardsTeamA;
    vector<string> redCardsTeamB;

    // Simular eventos para el equipo A
    for (Player& player : teamA.players) {
        if (simulateEvent(player.goalProbability)) {
            scoreA++;
            goalsTeamA.push_back(player.name);
        }
        if (simulateEvent(player.yellowCardProbability)) {
            player.yellowCards++;
            yellowCardsTeamA.push_back(player.name);
        }
        if (simulateEvent(player.redCardProbability)) {
            redCardsTeamA.push_back(player.name);
            if (!player.isSuspended) {
                player.isSuspended = true;  // Marcar al jugador como suspendido solo si no lo está ya
            }
        }
    }

    // Simular eventos para el equipo B
    for (Player& player : teamB.players) {
        if (simulateEvent(player.goalProbability)) {
            scoreB++;
            goalsTeamB.push_back(player.name);
        }
        if (simulateEvent(player.yellowCardProbability)) {
            player.yellowCards++;
            yellowCardsTeamB.push_back(player.name);
        }
        if (simulateEvent(player.redCardProbability)) {
            redCardsTeamB.push_back(player.name);
            if (!player.isSuspended) {
                player.isSuspended = true;  // Marcar al jugador como suspendido solo si no lo está ya
            }
        }
    }

    // Imprimir resultados del partido
    cout << teamA.name << " " << scoreA << " - " << scoreB << " " << teamB.name << "\n";

    // Imprimir goles anotados por cada equipo
    cout << "Goles Anotados por " << teamA.name << ":\n";
    for (const string& player : goalsTeamA) {
        cout << player << "\n";
    }
    cout << "\n";

    cout << "Goles Anotados por " << teamB.name << ":\n";
    for (const string& player : goalsTeamB) {
        cout << player << "\n";
    }
    cout << "\n";

    // Imprimir tarjetas amarillas y rojas
    cout << "Tarjetas amarillas " << teamA.name << ":\n";
    for (const string& player : yellowCardsTeamA) {
        cout << player << "\n";
    }
    cout << "\n";

    cout << "Tarjetas amarillas " << teamB.name << ":\n";
    for (const string& player : yellowCardsTeamB) {
        cout << player << "\n";
    }
    cout << "\n";

    cout << "Tarjetas rojas " << teamA.name << ":\n";
    for (const string& player : redCardsTeamA) {
        cout << player << "\n";
    }
    cout << "\n";

    cout << "Tarjetas rojas " << teamB.name << ":\n";
    for (const string& player : redCardsTeamB) {
        cout << player << "\n";
    }
    cout << "\n\n";

    // Determinar el resultado del partido basado en la probabilidad de victoria
    if (scoreA == scoreB) {
        // Si hay empate, simular penaltis
        cout << "Empate! Vamos a penaltis.\n";
        int penaltyResult = simulatePenalties(teamA, teamB);
        if (penaltyResult == 1) {
            teamA.points += 3;  // Equipo A gana en penaltis
        } else {
            teamB.points += 3;  // Equipo B gana en penaltis
        }
    } else if (simulateEvent(teamA.winProbability / (teamA.winProbability + teamB.winProbability))) {
        teamA.points += 3;  // Equipo A gana
    } else {
        teamB.points += 3;  // Equipo B gana (o empate)
    }
}


vector<Match> simulateQuarterFinals(vector<Group>& groups) {
    vector<Team*> quarterFinalTeams;

    // Seleccionar los dos equipos con más puntos de cada grupo
    for (Group& group : groups) {
        quarterFinalTeams.push_back(&group.teams[0]);
        quarterFinalTeams.push_back(&group.teams[1]);
    }

    // Mostrar los equipos clasificados a cuartos de final
    cout << "\nEquipos clasificados a cuartos de final:\n";
    for (Team* team : quarterFinalTeams) {
        cout << team->name << "\n";
    }
    cout << "\n";

    // Mezclar los equipos
    random_shuffle(quarterFinalTeams.begin(), quarterFinalTeams.end());

    // Crear los enfrentamientos de cuartos de final
    vector<Match> quarterFinalMatches;
    for (int i = 0; i < quarterFinalTeams.size(); i += 2) {
        Match match;
        match.teamA = quarterFinalTeams[i];
        match.teamB = quarterFinalTeams[i + 1];
        quarterFinalMatches.push_back(match);
    }

    // Mostrar los enfrentamientos de cuartos de final
    cout << "-----------CUARTOS DE FINAL------------\n";
    for (Match& match : quarterFinalMatches) {
        cout << match.teamA->name << " VS " << match.teamB->name << "\n";
    }
    cout << "\n";

    // Simular los partidos de cuartos de final
    for (Match& match : quarterFinalMatches) {
        simulateMatch(*match.teamA, *match.teamB);
       
    }
   
    return quarterFinalMatches;
}

vector<Team*> getQuarterFinalWinners(vector<Match>& quarterFinalMatches) {
    vector<Team*> winners;
    for (Match& match : quarterFinalMatches) {
        if (match.teamA->points > match.teamB->points) {
            winners.push_back(match.teamA);
        } else {
            winners.push_back(match.teamB);
        }
    }
    return winners;
}


vector<Match> simulateSemiFinals(vector<Match>& quarterFinalMatches) {
    vector<Team*> semiFinalTeams = getQuarterFinalWinners(quarterFinalMatches);
   
    // Mostrar los equipos clasificados a semifinales
    cout << "Equipos clasificados a semifinales:\n";
    for (Team* team : semiFinalTeams) {
        cout << team->name << "\n";
    }
    cout << "\n";

    // Mezclar los equipos
    random_shuffle(semiFinalTeams.begin(), semiFinalTeams.end());

    // Crear los enfrentamientos de semifinales
    vector<Match> semiFinalMatches;
    for (int i = 0; i < semiFinalTeams.size(); i += 2) {
        Match match;
        match.teamA = semiFinalTeams[i];
        match.teamB = semiFinalTeams[i + 1];
        semiFinalMatches.push_back(match);
    }

    // Mostrar los enfrentamientos de semifinales
    cout << "-----------SEMIFINALES----------\n";
    for (Match& match : semiFinalMatches) {
        cout << match.teamA->name << " VS " << match.teamB->name << "\n";
    }
    cout << "\n";

    // Simular los partidos de semifinales
    for (Match& match : semiFinalMatches) {
        simulateMatch(*match.teamA, *match.teamB);
    }
   
    // Devolver los enfrentamientos de semifinales
    return semiFinalMatches;
}

vector<Team*> getSemiFinalWinners(vector<Match>& semiFinalMatches) {
    vector<Team*> winners;
    for (Match& match : semiFinalMatches) {
        if (match.teamA->points > match.teamB->points) {
            winners.push_back(match.teamA);
        } else {
            winners.push_back(match.teamB);
        }
    }
    return winners;
}


void simulateFinal(vector<Match>& semiFinalMatches) {
    vector<Team*> finalTeams = getSemiFinalWinners(semiFinalMatches);

    // Mostrar los equipos clasificados a la final
    cout << "Equipos clasificados a la final:\n";
    for (Team* team : finalTeams) {
        cout << team->name << "\n";
    }
    cout << "\n";

    // Crear el enfrentamiento de la final
    vector<Match> finalMatch;
    Match match;
    match.teamA = finalTeams[0];
    match.teamB = finalTeams[1];
    finalMatch.push_back(match);

    // Mostrar el enfrentamiento de la final
    cout << "\n-------------FINAL-------------\n";
    cout << match.teamA->name << " VS " << match.teamB->name << "\n";
    cout << "\n";

    // Simular el partido de la final
    for (Match& match : finalMatch) {
        simulateMatch(*match.teamA, *match.teamB);
    }
}


void simulateGroupStage(vector<Group>& groups) {
    for (Group& group : groups) {
        cout << "\n" << group.name << endl;
        for (int i = 0; i < group.teams.size(); ++i) {
            for (int j = i + 1; j < group.teams.size(); ++j) {
                cout << group.teams[i].name << " VS " << group.teams[j].name << endl;
                simulateMatch(group.teams[i], group.teams[j]);
            }
        }
        // Ordenar los equipos del grupo por puntos (de mayor a menor)
        sort(group.teams.begin(), group.teams.end(), [](const Team& a, const Team& b) {
            return a.points > b.points;
        });
    }
}





void printResultsToFile(vector<Group>& groups) {
    ofstream outFile("results.txt");
    if (outFile.is_open()) {
        for (const Group& group : groups) {
            outFile << group.name << ":\n";
            for (const Team& team : group.teams) {
                outFile << "  - " << team.name << ": " << team.points << " puntos\n";
            }
            outFile << "\n";
        }
        outFile.close();
        cout << "Resultados guardados en results.txt\n";
    } else {
        cout << "Error al abrir el archivo para escritura.\n";
    }
}

int main() {
    srand(time(0));  // Inicializar la semilla aleatoria
   
    // Definir los equipos y asignar probabilidades de victoria
    vector<Team> teams(16);

            Team argentina;
            argentina.name = "Argentina";
            argentina.players.push_back({"Emiliano Martínez", 0.01, 0.5, 0.1});
            argentina.players.push_back({"Franco Armani", 0.01, 0.5, 0.0});
            argentina.players.push_back({"Juan Musso", 0.1, 0.4, 0.0});
            argentina.players.push_back({"Nicolas Otamendi", 0.1, 0.6, 0.1});
            argentina.players.push_back({"Cristian Romero", 0.2, 0.6, 0.1});
            argentina.players.push_back({"Germán Pezzella", 0.1, 0.6, 0.1});
            argentina.players.push_back({"Nicolás Tagliafico", 0.1, 0.5, 0.1});
            argentina.players.push_back({"Marcos Acuña", 0.1, 0.6, 0.1});
            argentina.players.push_back({"Nahuel Molina", 0.1, 0.5, 0.1});
            argentina.players.push_back({"Leandro Paredes", 0.2, 0.5, 0.1});
            argentina.players.push_back({"Rodrigo De Paul", 0.3, 0.6, 0.1});
            argentina.players.push_back({"Giovani Lo Celso", 0.2, 0.5, 0.1});
            argentina.players.push_back({"Exequiel Palacios", 0.2, 0.6, 0.1});
            argentina.players.push_back({"Ángel Di María", 0.3, 0.6, 0.1});
            argentina.players.push_back({"Guido Rodríguez", 0.2, 0.5, 0.1});
            argentina.players.push_back({"Lionel Messi", 0.5, 0.4, 0.1});
            argentina.players.push_back({"Lautaro Martínez", 0.4, 0.5, 0.1});
            argentina.players.push_back({"Ángel Correa", 0.4, 0.4, 0.1});
            argentina.players.push_back({"Nicolás González", 0.4, 0.5, 0.1});
            teams[0] = argentina;
            teams[0].winProbability = 0.7;
           
            Team mexico;
            mexico.name = "Mexico";
            mexico.players.push_back({"Guillermo Ochoa", 0.01, 0.02, 0.01});
            mexico.players.push_back({"Luis Ángel Malagón", 0.02, 0.4, 0.01});
            mexico.players.push_back({"Carlos Acevedo", 0.5, 0.4, 0.01});
            mexico.players.push_back({"Jesus Gallardo", 0.1, 0.5, 0.1});
            mexico.players.push_back({"César Montes", 0.1, 0.4, 0.1});
            mexico.players.push_back({"Jorge Sánchez", 0.1, 0.2, 0.1});
            mexico.players.push_back({"Gerardo Arteaga", 0.1, 0.5, 0.1});
            mexico.players.push_back({"Johan Vásquez", 0.1, 0.3, 0.1});
            mexico.players.push_back({"Kevin Álvarez", 0.1, 0.2, 0.1});
            mexico.players.push_back({"Ramón Juárez", 0.3, 0.3, 0.1});
            mexico.players.push_back({"Adrián Mora", 0.2, 0.6, 0.1});
            mexico.players.push_back({"Érick Sánchez", 0.2, 0.4, 0.1});
            mexico.players.push_back({"Luis Chávez", 0.3, 0.4, 0.1});
            mexico.players.push_back({"Luis Romo", 0.2, 0.3, 0.1});
            mexico.players.push_back({"Edson Álvarez", 0.2, 0.2, 0.1});
            mexico.players.push_back({"Orbelín Pineda", 0.3, 0.2, 0.1});
            mexico.players.push_back({"Roberto Alvarado", 0.3, 0.5, 0.1});
            mexico.players.push_back({"Diego Lainez", 0.2, 0.2, 0.1});
            mexico.players.push_back({"Jesús Corona", 0.3, 0.5, 0.1});
            mexico.players.push_back({"Raúl Jiménez", 0.4, 0.4, 0.1});
            teams[1] = mexico;
            teams[1].winProbability = 0.3;
           
            Team peru;
            peru.name = "Peru";
            peru.players.push_back({"Pedro Gallese", 0.02, 0.4, 0.01});
            peru.players.push_back({"Alejandro Duarte", 0.01, 0.3, 0.01});
            peru.players.push_back({"Carlos Cáceda", 0.1, 0.3, 0.01});
            peru.players.push_back({"Luis Abram", 0.1, 0.4, 0.1});
            peru.players.push_back({"Alexander Callens", 0.1, 0.4, 0.1});
            peru.players.push_back({"Carlos Zambrano", 0.2, 0.4, 0.1});
            peru.players.push_back({"Renato Tapia", 0.1, 0.3, 0.0});
            peru.players.push_back({"Miguel Trauco", 0.1, 0.3, 0.1});
            peru.players.push_back({"Marcos López", 0.1, 0.4, 0.1});
            peru.players.push_back({"Aldo Corzo", 0.1, 0.3, 0.0});
            peru.players.push_back({"Jhilmar Lora", 0.1, 0.3, 0.1});
            peru.players.push_back({"Christofer Gonzales", 0.2, 0.3, 0.0});
            peru.players.push_back({"Yoshimar Yotún", 0.2, 0.4, 0.1});
            peru.players.push_back({"Pedro Aquino", 0.2, 0.4, 0.1});
            peru.players.push_back({"Segio Peña", 0.3, 0.3, 0.1});
            peru.players.push_back({"Piero Quispe", 0.1, 0.3, 0.0});
            peru.players.push_back({"Horacio Calcaterra", 0.2, 0.3, 0.1});
            peru.players.push_back({"Cristian Benavente", 0.2, 0.3, 0.0});
            peru.players.push_back({"André Carillo", 0.3, 0.3, 0.0});
            peru.players.push_back({"Gianluca", 0.7, 0.2, 0.0});
            peru.players.push_back({"Christian Cueva", 0.6, 0.3, 0.1});
            peru.players.push_back({"Edison Flores", 0.4, 0.3, 0.0});
            peru.players.push_back({"Andy Apolo", 0.5, 0.3, 0.1});
            teams[2] = peru;
            teams[2].winProbability = 0.2;
           
            Team ecuador;
            ecuador.name = "Ecuador";
            ecuador.players.push_back({"Hernán Galíndez", 0.02, 0.4, 0.0});
            ecuador.players.push_back({"Alexander Domínguez", 0.02, 0.4, 0.01});
            ecuador.players.push_back({"Javier Burrai", 0.2, 0.3, 0.01});
            ecuador.players.push_back({"Piero Hincapié", 0.1, 0.5, 0.1});
            ecuador.players.push_back({"Willian Pacho", 0.2, 0.6, 0.1});
            ecuador.players.push_back({"Félix Torres", 0.1, 0.6, 0.1});
            ecuador.players.push_back({"Joel Ordóñez", 0.1, 0.5, 0.1});
            ecuador.players.push_back({"Pervis Estupiñán", 0.1, 0.6, 0.1});
            ecuador.players.push_back({"Diego Palacios", 0.1, 0.5, 0.1});
            ecuador.players.push_back({"Byron Castillo", 0.1, 0.6, 0.1});
            ecuador.players.push_back({"Jordan Plata", 0.1, 0.5, 0.1});
            ecuador.players.push_back({"Carlos Gruezo", 0.2, 0.5, 0.1});
            ecuador.players.push_back({"Moisés Caicedo", 0.3, 0.6, 0.1});
            ecuador.players.push_back({"Alan Franco", 0.2, 0.5, 0.1});
            ecuador.players.push_back({"Jeremy Sarmiento", 0.3, 0.6, 0.1});
            ecuador.players.push_back({"Ángel Mena", 0.2, 0.5, 0.1});
            ecuador.players.push_back({"Jhonny Arreaga", 0.3, 0.6, 0.1});
            ecuador.players.push_back({"Dixon Arce", 0.2, 0.5, 0.1});
            ecuador.players.push_back({"Wilder Delgado", 0.2, 0.6, 0.1});
            ecuador.players.push_back({"Enner Valencia", 0.5, 0.4, 0.1});
            ecuador.players.push_back({"Michael Estrada", 0.4, 0.5, 0.1});
            ecuador.players.push_back({"Marcelo Allende", 0.3, 0.5, 0.1});
            ecuador.players.push_back({"Leonardo Campana", 0.4, 0.4, 0.1});
            teams[3] = ecuador;
            teams[3].winProbability = 0.2;
           
            Team chile;
            chile.name = "Chile";
            chile.players.push_back({"Claudio Bravo", 0.02, 0.5, 0.01});
            chile.players.push_back({"Grabiel Arias", 0.02, 0.4, 0.01});
            chile.players.push_back({"Brayan Cortés", 0.02, 0.4, 0.0});
            chile.players.push_back({"Mauricio Isla", 0.1, 0.5, 0.1});
            chile.players.push_back({"Guillermo Maripán", 0.1, 0.6, 0.1});
            chile.players.push_back({"Enzo Roco", 0.2, 0.6, 0.1});
            chile.players.push_back({"Paulo Díaz", 0.1, 0.6, 0.1});
            chile.players.push_back({"Gabriel Suazo", 0.1, 0.5, 0.1});
            chile.players.push_back({"Diego Valderrama", 0.1, 0.6, 0.1});
            chile.players.push_back({"Valentin Cousiño", 0.1, 0.6, 0.1});
            chile.players.push_back({"Miiko Albornoz", 0.1, 0.5, 0.1});
            chile.players.push_back({"Charles Aránguiz", 0.2, 0.5, 0.1});
            chile.players.push_back({"Arturo Vidal", 0.2, 0.6, 0.1});
            chile.players.push_back({"Pedro Pablo Hernández", 0.2, 0.6, 0.1});
            chile.players.push_back({"Marcelino Núñez", 0.3, 0.5, 0.1});
            chile.players.push_back({"Claudio Baeza", 0.2, 0.5, 0.1});
            chile.players.push_back({"Ignacio Saavedra", 0.2, 0.6, 0.1});
            chile.players.push_back({"Diego Rubio", 0.3, 0.5, 0.1});
            chile.players.push_back({"Esteban Pavez", 0.2, 0.5, 0.1});
            chile.players.push_back({"Alexis Sánchez", 0.5, 0.4, 0.1});
            chile.players.push_back({"Ben Brereton Díaz", 0.4, 0.4, 0.1});
            chile.players.push_back({"Marcelo Allende", 0.3, 0.5, 0.1});
            chile.players.push_back({"Ángel Sagal", 0.4, 0.4, 0.1});
            teams[4] = chile;
            teams[4].winProbability = 0.4;
           
            Team venezuela;
            venezuela.name = "Venezuela";
            venezuela.players.push_back({"Wuilker Fariñez", 0.01, 0.5, 0.01});
            venezuela.players.push_back({"Joel Graterol", 0.01, 0.5, 0.01});
            venezuela.players.push_back({"Rafael Romo", 0.1, 0.4, 0.01});
            venezuela.players.push_back({"Nahuel Ferraresi", 0.1, 0.7, 0.1});
            venezuela.players.push_back({"Yordan Osorio", 0.1, 0.6, 0.1});
            venezuela.players.push_back({"Wilker Ángel", 0.2, 0.7, 0.1});
            venezuela.players.push_back({"Miquel Navarro", 0.1, 0.6, 0.1});
            venezuela.players.push_back({"Rochelle Celis", 0.1, 0.6, 0.1});
            venezuela.players.push_back({"Adrian Roja", 0.1, 0.6, 0.1});
            venezuela.players.push_back({"Luis Mago", 0.1, 0.6, 0.1});
            venezuela.players.push_back({"Robert Hernández", 0.1, 0.5, 0.1});
            venezuela.players.push_back({"Junior Moreno", 0.3, 0.6, 0.1});
            venezuela.players.push_back({"Yangel Herrera", 0.2, 0.6, 0.1});
            venezuela.players.push_back({"Darwin Machis", 0.2, 0.6, 0.1});
            venezuela.players.push_back({"Tomás Rincón", 0.2, 0.6, 0.1});
            venezuela.players.push_back({"José Cifuentes", 0.3, 0.6, 0.1});
            venezuela.players.push_back({"Érik Gutiérrez", 0.2, 0.5, 0.1});
            venezuela.players.push_back({"Anthony Martínez", 0.2, 0.6, 0.1});
            venezuela.players.push_back({"Francisco La Rosa", 0.2, 0.5, 0.1});
            venezuela.players.push_back({"Josef Martínez", 0.4, 0.5, 0.1});
            venezuela.players.push_back({"Yeferson Soteldo", 0.4, 0.5, 0.1});
            venezuela.players.push_back({"Sergio Córdova", 0.3, 0.5, 0.1});
            venezuela.players.push_back({"Salomón Rondón", 0.4, 0.4, 0.1});
            teams[5] = venezuela;
            teams[5].winProbability = 0.2;
           
            Team jamaica;
            jamaica.name = "Jamaica";
            jamaica.players.push_back({"Andre Blake", 0.01, 0.3, 0.01});
            jamaica.players.push_back({"Shamar Nicholson", 0.01, 0.4, 0.01});
            jamaica.players.push_back({"Dwayne Kerr", 0.01, 0.4, 0.01});
            jamaica.players.push_back({"Kemar Lawrence", 0.1, 0.5, 0.1});
            jamaica.players.push_back({"Damion Lowe", 0.1, 0.5, 0.1});
            jamaica.players.push_back({"Adrian Mariappa", 0.1, 0.5, 0.1});
            jamaica.players.push_back({"Oniel Fisher", 0.1, 0.4, 0.1});
            jamaica.players.push_back({"Shaquille Holness", 0.0, 0.5, 0.1});
            jamaica.players.push_back({"Alvas Powell", 0.1, 0.5, 0.1});
            jamaica.players.push_back({"Jamoi Jamaica", 0.0, 0.4, 0.1});
            jamaica.players.push_back({"Daniel Phillips", 0.0, 0.5, 0.1});
            jamaica.players.push_back({"Romaine Scott", 0.2, 0.5, 0.1});
            jamaica.players.push_back({"Gregore", 0.1, 0.5, 0.1});
            jamaica.players.push_back({"Jahmar Coombs", 0.1, 0.5, 0.1});
            jamaica.players.push_back({"Peter Vassell", 0.1, 0.4, 0.1});
            jamaica.players.push_back({"Javon Francis", 0.2, 0.6, 0.1});
            jamaica.players.push_back({"Shamar Nicholson", 0.2, 0.4, 0.1});
            jamaica.players.push_back({"Andre Gray", 0.2, 0.5, 0.1});
            jamaica.players.push_back({"Khimare Holness", 0.1, 0.5, 0.1});
            jamaica.players.push_back({"Dwayne Miller", 0.3, 0.4, 0.1});
            jamaica.players.push_back({"Cory Burke", 0.4, 0.5, 0.1});
            jamaica.players.push_back({"Nicholas Hamilton", 0.3, 0.4, 0.1});
            jamaica.players.push_back({"Junior Flemmings", 0.3, 0.4, 0.1});
            teams[6] = jamaica;
            teams[6].winProbability = 0.1;
           
            Team usa;
            usa.name = "USA";
            usa.players.push_back({"Zack Steffen", 0.02, 0.5, 0.1});
            usa.players.push_back({"Ethan Horvath", 0.02, 0.5, 0.01});
            usa.players.push_back({"Sean Johnson", 0.2, 0.4, 0.01});
            usa.players.push_back({"John Brooks", 0.1, 0.6, 0.1});
            usa.players.push_back({"Matt Miazga", 0.2, 0.6, 0.1});
            usa.players.push_back({"Sergiño Dest", 0.1, 0.6, 0.1});
            usa.players.push_back({"Antonee Robinson", 0.1, 0.5, 0.1});
            usa.players.push_back({"Aaron Long", 0.1, 0.6, 0.1});
            usa.players.push_back({"DeAndre Yedlin", 0.1, 0.5, 0.1});
            usa.players.push_back({"Weston McKennie", 0.2, 0.5, 0.1});
            usa.players.push_back({"Tyler Adams", 0.3, 0.6, 0.1});
            usa.players.push_back({"Christian Pulisic", 0.2, 0.5, 0.1});
            usa.players.push_back({"Gio Reyna", 0.2, 0.6, 0.1});
            usa.players.push_back({"Kellyn Acosta", 0.2, 0.6, 0.1});
            usa.players.push_back({"Jackson Yueill", 0.2, 0.5, 0.1});
            usa.players.push_back({"Brenden Aaronson", 0.5, 0.4, 0.1});
            usa.players.push_back({"Josh Sargent", 0.4, 0.5, 0.1});
            usa.players.push_back({"Tim Weah", 0.4, 0.4, 0.1});
            usa.players.push_back({"Jordan Pefok", 0.4, 0.5, 0.1});
            teams[7] = usa;
            teams[7].winProbability = 0.4;
           
            Team uruguay;
            uruguay.name = "Uruguay";
            uruguay.players.push_back({"Fernando Muslera", 0.02, 0.5, 0.1});
            uruguay.players.push_back({"Martín Campaña", 0.02, 0.5, 0.01});
            uruguay.players.push_back({"Sergio Rochet", 0.02, 0.4, 0.01});
            uruguay.players.push_back({"José María Giménez", 0.1, 0.6, 0.1});
            uruguay.players.push_back({"Diego Godín", 0.2, 0.6, 0.1});
            uruguay.players.push_back({"Giovanni González", 0.1, 0.6, 0.1});
            uruguay.players.push_back({"Matías Viña", 0.1, 0.5, 0.1});
            uruguay.players.push_back({"Martín Cáceres", 0.1, 0.6, 0.1});
            uruguay.players.push_back({"Ronald Araújo", 0.1, 0.5, 0.1});
            uruguay.players.push_back({"Federico Valverde", 0.2, 0.5, 0.1});
            uruguay.players.push_back({"Rodrigo Bentancur", 0.3, 0.6, 0.1});
            uruguay.players.push_back({"Nahitan Nández", 0.2, 0.5, 0.1});
            uruguay.players.push_back({"Matías Vecino", 0.2, 0.6, 0.1});
            uruguay.players.push_back({"Lucas Torreira", 0.2, 0.6, 0.1});
            uruguay.players.push_back({"Luis Suárez", 0.5, 0.4, 0.1});
            uruguay.players.push_back({"Edinson Cavani", 0.4, 0.5, 0.1});
            uruguay.players.push_back({"Maxi Gómez", 0.4, 0.4, 0.1});
            uruguay.players.push_back({"Jonathan Rodríguez", 0.5, 0.5, 0.1});
            teams[8] = uruguay;
            teams[8].winProbability = 0.5;
           
            Team colombia;
            colombia.name = "Colombia";
            colombia.players.push_back({"David Ospina", 0.02, 0.5, 0.01});
            colombia.players.push_back({"Álvaro Montero", 0.02, 0.5, 0.01});
            colombia.players.push_back({"Camilo Vargas", 0.2, 0.4, 0.01});
            colombia.players.push_back({"Davinson Sánchez", 0.1, 0.6, 0.1});
            colombia.players.push_back({"Yerry Mina", 0.2, 0.6, 0.1});
            colombia.players.push_back({"William Tesillo", 0.1, 0.6, 0.1});
            colombia.players.push_back({"Stefan Medina", 0.1, 0.5, 0.1});
            colombia.players.push_back({"Daniel Muñoz", 0.1, 0.6, 0.1});
            colombia.players.push_back({"Jhon Lucumí", 0.1, 0.5, 0.1});
            colombia.players.push_back({"Wilmar Barrios", 0.2, 0.5, 0.1});
            colombia.players.push_back({"Juan Guillermo Cuadrado", 0.3, 0.6, 0.1});
            colombia.players.push_back({"Mateus Uribe", 0.2, 0.5, 0.1});
            colombia.players.push_back({"Jefferson Lerma", 0.2, 0.6, 0.1});
            colombia.players.push_back({"Luis Díaz", 0.2, 0.6, 0.1});
            colombia.players.push_back({"Rafael Santos Borré", 0.5, 0.4, 0.1});
            colombia.players.push_back({"Duván Zapata", 0.4, 0.5, 0.1});
            colombia.players.push_back({"Luis Muriel", 0.4, 0.4, 0.1});
            colombia.players.push_back({"Alfredo Morelos", 0.5, 0.5, 0.1});
            teams[9] = colombia;
            teams[9].winProbability = 0.6;
           
            Team bolivia;
            bolivia.name = "Bolivia";
            bolivia.players.push_back({"Carlos Lampe", 0.01, 0.5, 0.01});
            bolivia.players.push_back({"Rubén Cordano", 0.01, 0.5, 0.01});
            bolivia.players.push_back({"Javier Rojas", 0.2, 0.4, 0.0});
            bolivia.players.push_back({"Adrián Jusino", 0.1, 0.6, 0.1});
            bolivia.players.push_back({"José María Carrasco", 0.2, 0.6, 0.1});
            bolivia.players.push_back({"Gabriel Valverde", 0.1, 0.6, 0.1});
            bolivia.players.push_back({"Diego Bejarano", 0.1, 0.5, 0.1});
            bolivia.players.push_back({"Luis Haquin", 0.1, 0.6, 0.1});
            bolivia.players.push_back({"Enrique Flores", 0.1, 0.5, 0.1});
            bolivia.players.push_back({"Erwin Saavedra", 0.2, 0.5, 0.1});
            bolivia.players.push_back({"Alejandro Chumacero", 0.3, 0.4, 0.2});
            bolivia.players.push_back({"Leonel Justiniano", 0.2, 0.5, 0.1});
            bolivia.players.push_back({"Ramiro Vaca", 0.2, 0.6, 0.1});
            bolivia.players.push_back({"Diego Wayar", 0.2, 0.5, 0.1});
            bolivia.players.push_back({"Juan Carlos Arce", 0.2, 0.5, 0.1});
            bolivia.players.push_back({"Marcelo Martins", 0.5, 0.4, 0.1});
            bolivia.players.push_back({"Carlos Saucedo", 0.4, 0.5, 0.1});
            bolivia.players.push_back({"Gilbert Álvarez", 0.4, 0.4, 0.1});
            bolivia.players.push_back({"Bruno Miranda", 0.4, 0.4, 0.1});
            teams[10] = bolivia;
            teams[10].winProbability = 0.3;
           
            Team panama;
            panama.name = "Panama";
            panama.players.push_back({"Luis Mejía", 0.02, 0.5, 0.1});
            panama.players.push_back({"Orlando Mosquera", 00.2, 0.4, 0.01});
            panama.players.push_back({"Marcos Allen", 0.2, 0.4, 0.01});
            panama.players.push_back({"Fidel Escobar", 0.1, 0.6, 0.1});
            panama.players.push_back({"Harold Cummings", 0.2, 0.6, 0.1});
            panama.players.push_back({"Éric Davis", 0.1, 0.6, 0.1});
            panama.players.push_back({"Francisco Palacios", 0.1, 0.5, 0.1});
            panama.players.push_back({"Michael Murillo", 0.1, 0.6, 0.1});
            panama.players.push_back({"Kevin Galván", 0.1, 0.5, 0.1});
            panama.players.push_back({"Aníbal Godoy", 0.2, 0.5, 0.1});
            panama.players.push_back({"Édgar Bárcenas", 0.3, 0.6, 0.1});
            panama.players.push_back({"Armando Cooper", 0.2, 0.5, 0.1});
            panama.players.push_back({"Abdiel Ayarza", 0.2, 0.6, 0.1});
            panama.players.push_back({"Alberto Quintero", 0.3, 0.6, 0.1});
            panama.players.push_back({"Omar Browne", 0.2, 0.5, 0.1});
            panama.players.push_back({"Gabriel Torres", 0.5, 0.4, 0.1});
            panama.players.push_back({"Rolando Blackburn", 0.4, 0.5, 0.1});
            panama.players.push_back({"José Fajardo", 0.4, 0.4, 0.1});
            panama.players.push_back({"Jorman Aguilar", 0.4, 0.5, 0.1});
            teams[11] = panama;
            teams[11].winProbability = 0.3;
           
            Team brasil;
            brasil.name = "Brasil";
            brasil.players.push_back({"Alisson", 0.02, 0.5, 0.1});
            brasil.players.push_back({"Ederson", 0.02, 0.5, 0.0});
            brasil.players.push_back({"Weverton", 0.2, 0.4, 0.0});
            brasil.players.push_back({"Thiago Silva", 0.1, 0.6, 0.1});
            brasil.players.push_back({"Marquinhos", 0.2, 0.6, 0.1});
            brasil.players.push_back({"Éder Militão", 0.1, 0.6, 0.1});
            brasil.players.push_back({"Danilo", 0.1, 0.5, 0.1});
            brasil.players.push_back({"Renan Lodi", 0.1, 0.6, 0.1});
            brasil.players.push_back({"Alex Telles", 0.1, 0.5, 0.1});
            brasil.players.push_back({"Casemiro", 0.2, 0.5, 0.1});
            brasil.players.push_back({"Fabinho", 0.3, 0.6, 0.1});
            brasil.players.push_back({"Fred", 0.2, 0.5, 0.1});
            brasil.players.push_back({"Everton Ribeiro", 0.2, 0.6, 0.1});
            brasil.players.push_back({"Lucas Paquetá", 0.3, 0.6, 0.1});
            brasil.players.push_back({"Philippe Coutinho", 0.2, 0.5, 0.1});
            brasil.players.push_back({"Neymar Jr.", 0.5, 0.4, 0.1});
            brasil.players.push_back({"Gabriel Jesus", 0.4, 0.5, 0.1});
            brasil.players.push_back({"Richarlison", 0.4, 0.4, 0.1});
            brasil.players.push_back({"Vinícius Júnior", 0.5, 0.5, 0.1});
            teams[12] = brasil;
            teams[12].winProbability = 0.8;
           
            Team paraguay;
            paraguay.name = "Paraguay";
            paraguay.players.push_back({"Antony Silva", 0.02, 0.5, 0.01});
            paraguay.players.push_back({"Gerardo Ortiz", 0.02, 0.4, 0.01});
            paraguay.players.push_back({"Miguel Martínez", 0.2, 0.4, 0.0});
            paraguay.players.push_back({"Gustavo Gómez", 0.1, 0.6, 0.1});
            paraguay.players.push_back({"Junior Alonso", 0.2, 0.6, 0.1});
            paraguay.players.push_back({"Juan Escobar", 0.1, 0.6, 0.1});
            paraguay.players.push_back({"Santiago Arzamendia", 0.1, 0.5, 0.1});
            paraguay.players.push_back({"Robert Rojas", 0.1, 0.6, 0.1});
            paraguay.players.push_back({"Fabián Balbuena", 0.1, 0.5, 0.1});
            paraguay.players.push_back({"Mathías Villasanti", 0.2, 0.5, 0.1});
            paraguay.players.push_back({"Óscar Romero", 0.3, 0.6, 0.1});
            paraguay.players.push_back({"Ángel Cardozo", 0.2, 0.5, 0.1});
            paraguay.players.push_back({"Gastón Giménez", 0.2, 0.6, 0.1});
            paraguay.players.push_back({"Richard Sánchez", 0.3, 0.6, 0.1});
            paraguay.players.push_back({"Alejandro Romero", 0.2, 0.5, 0.1});
            paraguay.players.push_back({"Miguel Almirón", 0.5, 0.4, 0.1});
            paraguay.players.push_back({"Ángel Romero", 0.4, 0.5, 0.1});
            paraguay.players.push_back({"Carlos González", 0.4, 0.4, 0.1});
            paraguay.players.push_back({"Antonio Sanabria", 0.4, 0.4, 0.1});
            teams[13] = paraguay;
            teams[13].winProbability = 0.4;
           
            Team costaRica;
            costaRica.name = "CostaRica";
            costaRica.players.push_back({"Keylor Navas", 0.02, 0.5, 0.1});
            costaRica.players.push_back({"Leonel Moreira", 0.02, 0.5, 0.0});
            costaRica.players.push_back({"Esteban Alvarado", 0.2, 0.4, 0.0});
            costaRica.players.push_back({"Kendall Waston", 0.1, 0.6, 0.1});
            costaRica.players.push_back({"Francisco Calvo", 0.2, 0.6, 0.1});
            costaRica.players.push_back({"Giancarlo González", 0.1, 0.6, 0.1});
            costaRica.players.push_back({"Oscar Duarte", 0.1, 0.5, 0.1});
            costaRica.players.push_back({"Cristian Gamboa", 0.1, 0.6, 0.1});
            costaRica.players.push_back({"Bryan Oviedo", 0.1, 0.5, 0.1});
            costaRica.players.push_back({"Celso Borges", 0.2, 0.5, 0.1});
            costaRica.players.push_back({"Yeltsin Tejeda", 0.3, 0.6, 0.1});
            costaRica.players.push_back({"Bryan Ruiz", 0.2, 0.5, 0.1});
            costaRica.players.push_back({"Joel Campbell", 0.2, 0.6, 0.1});
            costaRica.players.push_back({"Allan Cruz", 0.3, 0.6, 0.1});
            costaRica.players.push_back({"Ulises Segura", 0.2, 0.5, 0.1});
            costaRica.players.push_back({"Johan Venegas", 0.5, 0.4, 0.1});
            costaRica.players.push_back({"Ariel Rodríguez", 0.4, 0.5, 0.1});
            costaRica.players.push_back({"Marco Ureña", 0.4, 0.4, 0.1});
            costaRica.players.push_back({"Felicio Brown", 0.4, 0.5, 0.1});
            teams[14] = costaRica;
            teams[14].winProbability = 0.3;
           
           
            Team canada;
            canada.name = "Canada";
            canada.players.push_back({"Milan Borjan", 0.02, 0.4, 0.0});
            canada.players.push_back({"Dayne St.Clair", 0.02, 0.4, 0.0});
            canada.players.push_back({"MacIntyre", 0.1, 0.3, 0.0});
            canada.players.push_back({"Tajon Buchanan", 0.1, 0.5, 0.1});
            canada.players.push_back({"Kamal Miller", 0.2, 0.6, 0.1});
            canada.players.push_back({"Derek Cornelius", 0.1, 0.6, 0.1});
            canada.players.push_back({"Cyle Larin", 0.1, 0.5, 0.1});
            canada.players.push_back({"Richie Laryea", 0.1, 0.6, 0.1});
            canada.players.push_back({"Sam Adekugbe", 0.1, 0.5, 0.1});
            canada.players.push_back({"Justin Morrow", 0.2, 0.6, 0.1});
            canada.players.push_back({"Liam Fraser", 0.1, 0.5, 0.1});
            canada.players.push_back({"Atiba Hutchinson", 0.2, 0.5, 0.1});
            canada.players.push_back({"Jonathan Osorio", 0.2, 0.5, 0.1});
            canada.players.push_back({"Stephen Eustaquio", 0.2, 0.5, 0.1});
            canada.players.push_back({"Mark-Anthony Kaye", 0.2, 0.6, 0.1});
            canada.players.push_back({"Liam Millar", 0.2, 0.5, 0.1});
            canada.players.push_back({"Samuel Piette", 0.2, 0.5, 0.1});
            canada.players.push_back({"David Wotherspoon", 0.3, 0.6, 0.1});
            canada.players.push_back({"Ismael Koné", 0.2, 0.5, 0.1});
            canada.players.push_back({"Jonathan David", 0.5, 0.4, 0.1});
            canada.players.push_back({"Lucas Cavallini", 0.4, 0.5, 0.1});
            canada.players.push_back({"Ike Ugbo", 0.3, 0.4, 0.1});
            canada.players.push_back({"Theo Hernandez", 0.4, 0.4, 0.1});
            teams[15] = canada;
            teams[15].winProbability = 0.2;


    // Definir los grupos
    vector<Group> groups(4);
    for (int i = 0; i < 4; ++i) {
        groups[i].name = "Grupo " + string(1, 'A' + i);
    }

    // Asignar equipos a los grupos
    for (int i = 0; i < 16; ++i) {
        groups[i / 4].teams.push_back(teams[i]);
    }
   
    vector<Match> quarterFinalMatches;
    vector<Match> semiFinalMatches;

   
    int option;
    do {
        cout 
             << "1. Simular fase de grupos\n"
             << "2. Simular cuartos de final\n"
             << "3. Simular semifinales\n"
             << "4. Simular final\n"
             << "5. Imprimir grupos\n"
             << "10. Salir\n"
             << "Elige una opción: ";
        cin >> option;

        switch (option) {

       case 1:
            // Simular fase de grupos
            for (Group& group : groups) {
                cout << "\n" << group.name << endl;
                for (int i = 0; i < group.teams.size(); ++i) {
                    for (int j = i + 1; j < group.teams.size(); ++j) {
                        cout << group.teams[i].name << " VS " << group.teams[j].name << endl;
                        simulateMatch(group.teams[i], group.teams[j]);
                    }
                }
                // Ordenar los equipos del grupo por puntos (de mayor a menor)
                sort(group.teams.begin(), group.teams.end(), [](const Team& a, const Team& b) {
                    return a.points > b.points;
                });
            }
            break;
        case 2:
            // Simular cuartos de final
            quarterFinalMatches = simulateQuarterFinals(groups);
            break;
        case 3:
            // Simular semifinales
            semiFinalMatches = simulateSemiFinals(quarterFinalMatches);
            break;
        case 4:
            // Simular final
            simulateFinal(semiFinalMatches);
            break;
        case 5:
            // Imprimir resultados
            printResultsToFile(groups);
            break;
        case 10:
            // Salir
            break;
        default:
            cout << "Opción no válida. Por favor, elige una opción del menú.\n";
            break;
        }
    } while (option != 10);

    return 0;
}
