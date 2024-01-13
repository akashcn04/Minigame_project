#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct node
{
    int vertex;
    struct node *next;
} Node;

typedef struct graph
{
    int numVertices;
    Node *adjList[]; // array member for representing adjaceny list for each vertex......
} Graph;

Graph *createGraph(int numVertices)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph) + numVertices * sizeof(Node *));
    graph->numVertices = numVertices;

    for (int i = 0; i < numVertices; i++)
    {
        graph->adjList[i] = NULL;
    }

    return graph;
}
void addEdge(Graph *graph, int src, int dest)
{
    Node *existingNode = graph->adjList[src];

    if (existingNode == NULL)
    {

        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->vertex = dest;
        newNode->next = NULL;
        graph->adjList[src] = newNode;
    }
    else
    {

        existingNode->vertex = dest;
    }
}

int isSnakeOrLadder(Graph *graph, int position)
{
    Node *temp = graph->adjList[position];
    if (temp != NULL)
    {
        return temp->vertex;
    }

    return 0;
}

int movePlayer(Graph *graph, int currentPlayer, int roll)
{
    int newPosition = currentPlayer + roll;

    // Ensure the player cannot move past the 100th position
    if (newPosition > 100)
    {
        printf("Cannot move past 100. Stay at the current position %d.\n", currentPlayer);
        return currentPlayer;
    }

    int snakeOrLadder = isSnakeOrLadder(graph, newPosition);

    if (snakeOrLadder != 0)
    {
        if (snakeOrLadder < newPosition)
        {
            printf("Player has been bitten by a snake at position! %d\n", newPosition);
        }
        else if (snakeOrLadder > newPosition)
        {
            printf("you are lucky! you got a ladder at %d\n", newPosition);
        }
        return snakeOrLadder;
    }
    else
    {
        return newPosition;
    }
}
void printBoard(int player1, int player2, int player3)
{
    int board[101];
    for (int i = 1; i <= 100; i++)
    {
        board[i] = i;
    }

    int playersPos[101][4] = {0}; // Array to store player positions
    playersPos[player1][0] = 1;   // Mark Player 1's position
    playersPos[player2][1] = 1;   // Mark Player 2's position
    playersPos[player3][2] = 1;   // Mark Player 3's position

    for (int i = 1; i <= 100; i++)
    {
        int count = 0;

        if (playersPos[i][0] == 1)
        {
            printf("#P1");
            count++;
        }

        if (playersPos[i][1] == 1)
        {
            if (count > 0)
            {
                printf(",P2");
            }
            else
            {
                printf("#P2");
            }
            count++;
        }

        if (playersPos[i][2] == 1)
        {
            if (count > 0)
            {
                printf(",P3");
            }
            else
            {
                printf("#P3");
            }
            count++;
        }

        if (count == 0)
        {
            printf("%d", board[i]);
        }

        printf(" ");

        if (i % 10 == 0)
        {
            printf("\n\n");
        }
    }
    printf("\n");
}

int rollDie()
{
    return rand() % 6 + 1;
}

void initializeGraph(Graph *graph)
{
    addEdge(graph, 6, 40);
    addEdge(graph, 23, 13);
    addEdge(graph, 45, 38);
    addEdge(graph, 61, 43);
    addEdge(graph, 65, 57);
    addEdge(graph, 77, 82);
    addEdge(graph, 98, 88);
}

void printGameStatus(int player1Position, int player2Position, int player3Position)
{

    printf("Player 1 is at position %d\n", player1Position);
    printf("Player 2 is at position %d\n", player2Position);
    printf("Player 3 is at position %d\n", player3Position);

    printf("|-------------------------------------------------|\n");
}

int main()
{
    srand(time(0));

    Graph *graph = createGraph(101);

    initializeGraph(graph);
    int numPlayers;

    // Ask for the number of players
    do
    {
        printf("Enter the number of players (2 or 3): ");
        scanf("%d", &numPlayers);
    } while (numPlayers != 2 && numPlayers != 3);

    int player1Position = 0;
    int player2Position = 0;
    int player3Position = 0; // Added for 3 players
    int won = 0;

    printf("Snake and Ladder Game\n");

    while (!won)
    {
        // Player 1's turn
        printf("\nPlayer 1, press Enter to roll the die...");
        while (getchar() != '\n')
            ;

        int roll = rollDie();
        printf("You rolled a %d.\n", roll);

        player1Position = movePlayer(graph, player1Position, roll);

        printBoard(player1Position, player2Position, player3Position);
        printGameStatus(player1Position, player2Position, player3Position);

        if ((numPlayers == 2 || numPlayers == 3) && player1Position == graph->numVertices - 1)
        {
            printf("Player 1 wins!\n");
            won = 1;
            break;
        }

        // Player 2's turn
        printf("\nPlayer 2, press Enter to roll the die...");
        while (getchar() != '\n')
            ;

        roll = rollDie();
        printf("You rolled a %d.\n", roll);

        player2Position = movePlayer(graph, player2Position, roll);

        printBoard(player1Position, player2Position, player3Position);
        printGameStatus(player1Position, player2Position, player3Position);

        if ((numPlayers == 2 || numPlayers == 3) && player2Position == graph->numVertices - 1)
        {
            printf("Player 2 wins!\n");
            won = 1;
        }

        if (numPlayers == 3 && won != 1)
        {
            // Player 3's turn
            printf("\nPlayer 3, press Enter to roll the die...");
            while (getchar() != '\n')
                ;

            roll = rollDie();
            printf("You rolled a %d.\n", roll);

            player3Position = movePlayer(graph, player3Position, roll);

            printBoard(player1Position, player2Position, player3Position);
            printGameStatus(player1Position, player2Position, player3Position);

            if (player3Position == graph->numVertices - 1)
            {
                printf("Player 3 wins!\n");
                won = 1;
            }
        }
    }

    return 0;
}