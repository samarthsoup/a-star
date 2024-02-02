#include <iostream>
#include <queue>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <climits>
using namespace std;

#define n 3
#define max_children 4

struct Node {
    Node* parent;
    int matrix[n][n];
    int x, y;
    int heuristic_cost;
    int depth;
};

void printMatrix(int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

Node* newNode(
    int matrix[n][n],
    int x,
    int y,
    int new_x,
    int new_y,
    int depth,
    Node* parent
) {
    Node* node = new Node;

    node->parent = parent;

    memcpy(node->matrix, matrix, sizeof node->matrix);
    swap(node->matrix[x][y], node->matrix[new_x][new_y]);
    node->heuristic_cost = INT_MAX;
    node->depth = depth;

    node->x = new_x;
    node->y = new_y;

    return node;
}

int row[] = { 1, 0, -1, 0 };
int col[] = { 0, -1, 0, 1 };

int calculateCost(int matrix[n][n], int final[n][n]) {
    int cost = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0) {
                int goalX = (matrix[i][j] - 1) / n;
                int goalY = (matrix[i][j] - 1) % n;
                cost += abs(i - goalX) + abs(j - goalY);
            }
        }
    }
    return cost;
}

bool ifSafe(int x, int y) {
    return (x >= 0 && x < n && y >= 0 && y < n);
}

void printPath(Node* root) {
    if (root == NULL) return;

    printPath(root->parent);
    printMatrix(root->matrix);

    printf("\n");
}

struct comp {
    bool operator()(const Node* lhs, const Node* rhs) const {
        return (lhs->heuristic_cost + lhs->depth) > (rhs->heuristic_cost + rhs->depth);
    }
};

void solve(int initial[n][n], int x, int y, int final[n][n]) {
    priority_queue<Node*, std::vector<Node*>, comp> pq;
    Node* root = newNode(initial, x, y, x, y, 0, NULL);
    root->heuristic_cost = calculateCost(initial, final);

    pq.push(root);

    while (!pq.empty()) {
        Node* min = pq.top();

        pq.pop();

        if (min->heuristic_cost == 0) {
            printPath(min);
            return;
        }

        for (int i = 0; i < max_children; i++) {
            if (ifSafe(min->x + row[i], min->y + col[i])) {
                Node* child = newNode(min->matrix, min->x, min->y, min->x + row[i], min->y + col[i], min->depth + 1, min);
                child->heuristic_cost = calculateCost(child->matrix, final);
                pq.push(child);
            }
        }
    }

    cout << "No solution found!" << endl;
}

int main() {
    int initial[n][n] = {
        {1, 2, 3},
        {5, 6, 0},
        {7, 8, 4}
    };

    int final[n][n] = {
        {1, 2, 3},
        {5, 6, 8},
        {0, 7, 4}
    };

    int x = 1, y = 2;

    solve(initial, x, y, final);

    return 0;
}
