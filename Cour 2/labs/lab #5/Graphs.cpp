/*
Блок 0: (1, 2
Блок 1: 5, 6), 7
Блок 2: 11 
Блок 3: 14, 15
Блок 4: 18
Блок 5: 19
Блок 6: 21
*/

#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <thread>
#include <string>
#include <benchmark.h>

#define INF 1e9
#define bm_ver 100
#define bm_ed 400
#define bm_mw 100
#define bm true


struct edge {
    int is, to;
    double dis;
    edge(int is, int to, double dis) : is(is), to(to), dis(dis) {};
};

class adj_list_graph {
public:
    int num_vertex;
    std::list<std::pair<int, double>>* adj_list;
    bool directed;

    adj_list_graph(int num_vertices = 0, bool is_directed = false) : num_vertex(num_vertices), directed(is_directed) {
        adj_list = new std::list<std::pair<int, double>>[num_vertex];
    }

    ~adj_list_graph() {
        delete[] adj_list;
    }

    adj_list_graph& operator=(const adj_list_graph& other) {
        if (this == &other) 
            return *this;

        num_vertex = other.num_vertex;
        directed = other.directed;

        delete[] adj_list;

        adj_list = new std::list<std::pair<int, double>>[num_vertex];
        for (int i = 0; i < num_vertex; ++i) {
            adj_list[i] = other.adj_list[i];
        }

        return *this;
    }

    //Додавання ребра
    void add_edge(int u, int v, double weight) {
        if (u >= 0 && u < num_vertex && v >= 0 && v < num_vertex && !edge_exists(u, v)) {
            adj_list[u].push_back(std::make_pair(v, weight));
            if (!directed)
                adj_list[v].push_back(std::make_pair(u, weight));
        }
        else {
            std::cout << "Invalid or duplicate edge: (" << u << ", " << v << ")." << std::endl;
        }
    }

    //Видалення ребра
    void remove_edge(int u, int v) {
        if (u >= 0 && u < num_vertex && v >= 0 && v < num_vertex) {
            adj_list[u].remove_if([&](const std::pair<int, double>& element) {
                return element.first == v;
                });
            if (!directed)
                adj_list[v].remove_if([&](const std::pair<int, double>& element) {
                return element.first == u;
                    });
        }
        else {
            std::cout << "Invalid vertices: (" << u << ", " << v << ")." << std::endl;
        }
    }

    //Вивід графу 
    void print_graph() {
        for (int i = 0; i < num_vertex; ++i) {
            std::cout << "Adjacency list of vertex " << i << ": ";
            for (auto it = adj_list[i].begin(); it != adj_list[i].end(); ++it) {
                std::cout << "(" << it->first << ", " << it->second << ")";
                if (std::next(it) != adj_list[i].end()) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
    }

    //Створення випадковго графу з n вершинами
    void create_random_graph(int n, double max_weight) {
        int edges_added = 0;
        while (edges_added < n) {
            int u = std::rand() % num_vertex;
            int v = std::rand() % num_vertex;
            if (u != v && !edge_exists(u, v)) {
                double raw_weight = static_cast<double>(std::rand()) / (static_cast<double>(RAND_MAX / max_weight));
                double weight = round(raw_weight * 10) / 10.0; // Round to one decimal place
                add_edge(u, v, weight);
                edges_added++;
            }
        }
    }


    //Перевірка чи граф зв'язний
    bool is_connected() {
        if (directed) {
            std::cout << "Is directed graph.\n";
            return false;
        }

        std::vector<bool> visited(num_vertex, false);

        dfs_connected(0, visited);
        for (auto x : visited) {
            if (x == false) return x;
        }
        return true;
    }

    //Пошук компонент зв'язності
    void find_connected_components(bool benchmark = false) {
        if (directed) {
            std::cout << "Is directed graph.\n";
            return;
        }

        std::vector<std::vector<int>> components;
        std::vector<bool> visited(num_vertex, false);
        for (int i = 0; i < num_vertex; ++i) {
            if (!visited[i]) {
                std::vector<int> component;
                dfs_connected_components(i, visited, component);
                components.push_back(component);
            }
        }
        if (!benchmark) {
            for (size_t i = 0; i < components.size(); ++i) {
                std::cout << i + 1 << " component: ";
                for (size_t j = 0; j < components[i].size(); ++j) {
                    std::cout << components[i][j] << (j == components[i].size() - 1 ? "" : ", ");
                }
                std::cout << std::endl;
            }
        }
    }

    //Перевірка графа на ациклічність
    bool is_acyclic() {
        if (!directed) {
            std::cout << "Is undirected graph.\n";
            return true;
        }

        std::vector<int> visited(num_vertex, 0);
        bool has_cycle = false;
        for (int v = 0; v < num_vertex; ++v) {
            if (!visited[v]) {
                dfs_cycle(visited, v, has_cycle);
            }
        }
        return !has_cycle;
    }

    //Пошук в глибину 
    void DFS(int start_vertex = 0, bool benchmark = false) {
        std::vector<bool> visited(num_vertex, false);
        DFS_util(start_vertex, visited, benchmark);
        for (int i = 0; i < num_vertex; ++i) {
            if (!visited[i]) DFS_util(i, visited, benchmark);
        }
    }

    //Пошук в глибину за найменшою вагою ребра
    void dfs_min_weight(bool benchmark = false) {
        std::vector<bool> visited(num_vertex, false);
        for (int i = 0; i < num_vertex; i++) {
            if (!visited[i]) {
                dfs_min_weight_helper(visited, i, -1, INF, benchmark);
            }
        }
        if (!benchmark) std::cout << std::endl;
    }

    //Алгорит Дейкстри для пошуку найменшої відстані між однією вершиною з усіма іншими
    void dijkstra(int source, bool benchmark = false) {
        std::vector<double> dist(num_vertex, INF);
        std::queue<int> Q;

        dist[source] = 0;
        Q.push(source);

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (const auto& edge : adj_list[u]) {
                int v = edge.first;
                double weight = edge.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    Q.push(v);
                }
            }
        }

        if (!benchmark) {
            std::cout << "Vertex \t Distance from Source\n";
            for (int i = 0; i < num_vertex; ++i) {
                std::cout << i << " \t ";
                (dist[i] != INF ? std::cout << dist[i] : std::cout << "Path don`t exist");
                std::cout << std::endl;
            }
        }

    }

    //Топологічне сортування
    void topological_sort(bool benchmark = false) {
        if (!directed) {
            std::cout << "Is undirected graph.\n";
            return;
        }

        std::stack<int> Stack;
        std::vector<bool> visited(num_vertex, false);

        for (int i = 0; i < num_vertex; ++i) {
            if (!visited[i]) {
                topologica_sort_util(i, visited, Stack);
            }
        }

        while (!Stack.empty() && !benchmark) {
            std::cout << Stack.top() << " ";
            Stack.pop();
        }
    }

    //Алгоритм побудови кістякового дерева за допомогою пошука в глибину
    void build_spanning_forest(adj_list_graph& forest) {
        forest.num_vertex = num_vertex;
        forest.adj_list = new std::list<std::pair<int, double>>[num_vertex];
        forest.directed = !directed;
        if (!is_connected()) {
            std::cout << "Graph isn't connected.\n";
            return;
        }

        std::vector<bool> visited(num_vertex, false);
        for (int i = 0; i < num_vertex; ++i) {
            if (!visited[i]) {
                build_spanning_forest_dfs(i, visited, forest);
            }
        }
    }

    //Алгоритм Крускала для побудови кістякового дерева найменшої ваги
    void kruskal(bool benchmark = false) {
        if (!is_connected()) {
            std::cout << "Graph isn`t connected.\n";
            return;
        }

        std::vector<edge> edges = get_edges();
        std::vector<int> parent(num_vertex); // To keep track of the parent of each vertex
        for (int i = 0; i < num_vertex; ++i) {
            parent[i] = i;
        }

        int min_weight = 0; // Total weight of the minimum spanning tree

        for (const auto& edge : edges) {
            int rootX = find(parent, edge.is);
            int rootY = find(parent, edge.to);
            if (rootX != rootY) { // If including this edge doesn't form a cycle
                if (!benchmark) std::cout << edge.is << "->" << edge.to << " (" << edge.dis << ")\n";
                min_weight += edge.dis;
                union_sets(parent, edge.is, edge.to);
            }
        }
        if (!benchmark) std::cout << "Total weight of MST: " << min_weight << std::endl;
    }

private:
    //Допоміжна функція перевірки існування ребра (додавання та видалення ребра)
    bool edge_exists(int u, int v) {
        if (u >= num_vertex || v >= num_vertex)
            return false;

        for (auto& edge : adj_list[u]) {
            if (edge.first == v)
                return true;
        }

        return false;
    }

    //Допоміжна функція для проходу графа в глибину за вагою ребра
    void dfs_min_weight_helper(std::vector<bool>& visited, int v, int prev_vertex, double prev_weight, bool benchmark) {
        visited[v] = true;
        if (!benchmark)std::cout << v << " ";

        std::vector<std::pair<int, double>> sorted_neighbors(adj_list[v].begin(), adj_list[v].end());
        std::sort(sorted_neighbors.begin(), sorted_neighbors.end(), [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
            return a.second < b.second; 
            });

        for (const auto& neighbor : sorted_neighbors) {
            if (!visited[neighbor.first]) {
                dfs_min_weight_helper(visited, neighbor.first, v, neighbor.second, benchmark);
            }
        }
    }

    //Пошук ребра мінімальної ваги
    std::pair<int, double> find_min_weight_edge(int v, int prev_vertex, double prev_weight) {
        std::pair<int, double> min_edge = std::make_pair(-1, INF);

        for (auto neighbor : adj_list[v]) {
            if (neighbor.second < min_edge.second && neighbor.first != prev_vertex && neighbor.second > prev_weight) {
                min_edge = neighbor;
            }
        }

        return min_edge;
    }

    //Допоміжна функція для перевірки ациклічності орієнтованого графа, пошук циклу
    void dfs_cycle(std::vector<int>& visited, int v, bool& has_cycle) {
        visited[v] = 1;

        for (auto neighbor : adj_list[v]) {
            if (visited[neighbor.first] == 0) {
                dfs_cycle(visited, neighbor.first, has_cycle);
            }
            else if (visited[neighbor.first] == 1) {
                has_cycle = true;
            }
        }
        visited[v] = 2;
    }

    //Допоміжна функція для пошуку компонент зв'язності
    void dfs_connected_components(int v, std::vector<bool>& visited, std::vector<int>& component) {
        visited[v] = true;
        component.push_back(v);
        for (auto neighbor : adj_list[v]) {
            if (!visited[neighbor.first]) {
                dfs_connected_components(neighbor.first, visited, component);
            }
        }
    }

    //Допоміжна функція для перевірки зв'язності
    void dfs_connected(int v, std::vector<bool>& visited) {
        visited[v] = true;
        for (auto neighbor : adj_list[v]) {
            if (!visited[neighbor.first]) {
                dfs_connected(neighbor.first, visited);
            }
        }
    }

    //Допоміжна функція для топологічного сортування
    void topologica_sort_util(int v, std::vector<bool>& visited, std::stack<int>& Stack) {
        visited[v] = true;

        for (auto& neighbor : adj_list[v]) {
            if (!visited[neighbor.first]) {
                topologica_sort_util(neighbor.first, visited, Stack);
            }
        }

        Stack.push(v);
    }

    //Допоміжна функція для побудови простого кістякового дерева
    void build_spanning_forest_dfs(int start, std::vector<bool>& visited, adj_list_graph& forest) {
        visited[start] = true;
        for (auto& neighbor : adj_list[start]) {
            int v = neighbor.first;
            if (!visited[v]) {
                forest.add_edge(start, v, neighbor.second);
                build_spanning_forest_dfs(v, visited, forest);
            }
        }
    }

    //Функція для отримання усіх ребр графа
    std::vector<edge> get_edges() {
        std::vector<edge> edges;
        for (int i = 0; i < num_vertex; ++i) {
            for (auto& neighbor : adj_list[i]) {
                edges.push_back(edge(i, neighbor.first, neighbor.second));
            }
        }

        std::sort(edges.begin(), edges.end(), [](const edge& a, const edge& b) {
            return a.dis < b.dis;
            });

        return edges;
    }

    int find(std::vector<int>& parent, int vertex) {
        if (parent[vertex] != vertex)
            parent[vertex] = find(parent, parent[vertex]);
        return parent[vertex];
    }

    void union_sets(std::vector<int>& parent, int x, int y) {
        int rootX = find(parent, x);
        int rootY = find(parent, y);
        parent[rootX] = rootY;
    }

    void DFS_util(int v, std::vector<bool>& visited, bool benchmark) {
        visited[v] = true;
        if (!benchmark) std::cout << v << " ";

        for (auto& neighbor : adj_list[v]) {
            int adj_vertex = neighbor.first;
            if (!visited[adj_vertex]) {
                DFS_util(adj_vertex, visited, benchmark);
            }
        }
    }
};

class adj_matrix_graph {
public:
    int num_vertex;
    std::vector<std::vector<double>> adj_matrix;
    bool directed;

    adj_matrix_graph(int num_vertices = 0, bool is_directed = false) : num_vertex(num_vertices), directed(is_directed) {
        adj_matrix.resize(num_vertex);
        for (int i = 0; i < num_vertex; i++) {
            adj_matrix[i].resize(num_vertex, 0);
        }
    }

    adj_matrix_graph& operator=(const adj_matrix_graph& other) {
        if (this == &other) 
            return *this;

        num_vertex = other.num_vertex;
        directed = other.directed;
        adj_matrix = other.adj_matrix; 

        return *this;
    }

    //Додавання ребра до графу
    void add_edge(int u, int v, double weight) {
        if (u >= 0 && u < num_vertex && v >= 0 && v < num_vertex && !edge_exists(u, v)) {
            adj_matrix[u][v] = weight;
            if (!directed)
                adj_matrix[v][u] = weight;
        }
        else {
            std::cout << "Invalid or duplicate edge: (" << u << ", " << v << ")." << std::endl;
        }
    }

    //Видалення ребра з графу
    void remove_edge(int u, int v) {
        if (u >= 0 && u < num_vertex && v >= 0 && v < num_vertex && edge_exists(u, v)) {
            adj_matrix[u][v] = 0;
            if (!directed)
                adj_matrix[v][u] = 0;
        }
        else {
            std::cout << "Invalid or non-existent edge: (" << u << ", " << v << ")." << std::endl;
        }
    }

    //Вивід графу (матриці суміжності)
    void print_graph() {
        for (int i = 0; i < num_vertex; ++i) {
            std::cout << "Vertex " << i << ": ";
            for (int j = 0; j < num_vertex; ++j) {
                std::cout << std::setw(8) << adj_matrix[i][j];
            }
            std::cout << std::endl;
        }
    }

    //Створення випадкового графу з n ребрами 
    void create_random_graph(int n, int max_weight) {
        if (n <= 0 || max_weight < 0) {
            std::cout << "Error: Invalid parameters.\n";
            return;
        }

        int edges_added = 0;
        while (edges_added < n) {
            int u = std::rand() % num_vertex;
            int v = std::rand() % num_vertex;
            float raw_weight = 1.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / max_weight));
            float weight = roundf(raw_weight * 10) / 10;

            if (u != v && !edge_exists(u, v) && !edge_exists(v, u)) {
                add_edge(u, v, weight);
                edges_added++;
            }
        }
    }

    //Перевірка на зв'язність графу 
    bool is_connected() {
        if (directed) {
            std::cout << "Is directed graph." << std::endl;
            return false;
        }

        std::vector<bool> visited(num_vertex, false);
        dfs_connected(0, visited);
        for (auto x : visited) {
            if (x == false) return x;
        }
        return true;
    }

    //Пошук усіх компонент зв'язності
    void find_connected_components(bool benchmark = false) {
        if (directed) {
            std::cout << "Is directed graph." << std::endl;
            return;
        }
        std::vector<std::vector<int>> components;
        std::vector<bool> visited(num_vertex, false);
        for (int i = 0; i < num_vertex; ++i) {
            if (!visited[i]) {
                std::vector<int> component;
                dfs_connected_components(i, visited, component);
                components.push_back(component);
            }
        }

        if (!benchmark) {
            for (size_t i = 0; i < components.size(); ++i) {
                std::cout << i + 1 << " component: ";
                for (size_t j = 0; j < components[i].size(); ++j) {
                    std::cout << components[i][j] << (j == components[i].size() - 1 ? "" : ", ");
                }
                std::cout << std::endl;
            }
        }
    }

    //Перевірка ациклічності орієнтованого графа 
    bool is_acyclic() {
        if (!directed) {
            std::cout << "Is undirected graph." << std::endl;
            return true;
        }

        std::vector<int> visited(num_vertex, 0);
        bool has_cycle = false;
        for (int v = 0; v < num_vertex; ++v) {
            if (!visited[v]) {
                dfs_cycle(visited, v, has_cycle);
            }
        }
        return !has_cycle;
    }
    
    //Пошук в глибину 
    void DFS(int vertex = 0, bool benchmark = false) {
        std::vector<bool> visited(num_vertex, false);
        DFS_util(vertex, visited, benchmark);
        for (int i = 0; i < num_vertex; ++i) {
            if (!visited[i]) DFS_util(i, visited, benchmark);
        }
    }

    //Прохід графа в глибину за найменшою вагою ребра
    void dfs_min_weight(bool benchmark = false) {
        std::vector<bool> visited(num_vertex, false);
        for (int i = 0; i < num_vertex; i++) {
            if (!visited[i]) {
                dfs_min_weight_helper(visited, i, -1, INF, benchmark);
            }
        }
        if (!benchmark) std::cout << std::endl;
    }

    //Алгоритм Флойда для знаходження найменшої відстані між усіма вершинами
    void al_floyde(bool benchmark = false) {
        std::vector<std::vector<double>> dis;
        dis = adj_matrix;

        for (int i = 0; i < num_vertex; ++i) {
            for (int j = 0; j < num_vertex; ++j) {
                if (i == j) dis[i][j] = 0;
                else if (dis[i][j] == 0) dis[i][j] = INF;
            }
        }

        for (int k = 0; k < num_vertex; ++k) {
            for (int i = 0; i < num_vertex; ++i) {
                for (int j = 0; j < num_vertex; ++j) {
                    if (dis[i][k] != INF && dis[k][j] != INF && dis[i][k] + dis[k][j] < dis[i][j]) {
                        dis[i][j] = dis[i][k] + dis[k][j];
                    }
                }
            }
        }

        if (!benchmark) {
            std::cout << "Distance: \n";
            for (int i = 0; i < num_vertex; ++i) {
                std::cout << "Vertex " << i << ": ";
                for (int j = 0; j < num_vertex; ++j) {
                    std::cout << std::setw(8) << adj_matrix[i][j];
                }
                std::cout << std::endl;
            }
        }

    }

    //Алгорит Дейкстри для пошуку наймешої відстані між однією вершиною і усіма іншими
    void dijkstra(int start, bool benchmark = false) {
        if (start < 0 || start >= num_vertex) {
            std::cout << "Error: Invalid vertex.\n";
            return;
        }

        std::vector<double> dist(num_vertex, INF);
        dist[start] = 0;
        std::vector<bool> visited(num_vertex, false);

        for (int count = 0; count < num_vertex - 1; ++count) {
            int u = min_distance(dist, visited);

            visited[u] = true;
            for (int v = 0; v < num_vertex; ++v) {
                if (!visited[v] && adj_matrix[u][v] && dist[u] != INF && dist[u] + adj_matrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adj_matrix[u][v];
                }
            }
        }

        if (!benchmark) {
            std::cout << "Vertex \t Distance from Source\n";
            for (int i = 0; i < num_vertex; ++i) {
                std::cout << i << " \t ";
                (dist[i] != INF ? std::cout << dist[i] : std::cout << "Path don`t exist");
                std::cout << std::endl;
            }
        }
    }

    //Топологічне сортування
    void topological_sort(bool benchmark = false) {
        if (!directed) {
            std::cout << "Is undirected graph.\n";
            return;
        }

        std::stack<int> Stack;
        std::vector<bool> visited(num_vertex, false);

        for (int i = 0; i < num_vertex; i++)
            if (!visited[i])
                topological_sort_util(i, visited, Stack);

        while (!Stack.empty() && !benchmark) {
            std::cout << Stack.top() << " ";
            Stack.pop();
        }
    }

    //Алгоритм побудови довільного кістякового дерева за допомогою пошука в глибину
    adj_matrix_graph build_spanning_forest() {
        if (!is_connected()) {
            std::cout << "Graph isn`t connected.\n";
            return adj_matrix_graph();
        }

        std::vector<bool> visited(num_vertex, false);
        adj_matrix_graph forest(num_vertex, !directed);
        for (int i = 0; i < num_vertex; ++i) {
            if (!visited[i]) {
                build_spanning_forest_dfs(i, visited, forest);
            }
        }
        return forest;
    }

    void kruskal(bool benchmark = false) {
        if (!is_connected()) {
            std::cout << "Graph isn`t connected.\n";
            return;
        }

        std::vector<edge> edges = get_edges();
        std::vector<int> parent(num_vertex); 
        for (int i = 0; i < num_vertex; ++i) {
            parent[i] = i;
        }

        double min_weight = 0; 

        for (const auto& edge : edges) {
            int rootX = find(parent, edge.is);
            int rootY = find(parent, edge.to);
            if (rootX != rootY) { 
                if (!benchmark) std::cout << edge.is << "->" << edge.to << " (" << edge.dis << ")\n";
                min_weight += edge.dis;
                union_sets(parent, edge.is, edge.to);
            }
        }
        if (!benchmark) std::cout << "Total weight of MST: " << min_weight << std::endl;
    }

private:
    int min_distance(std::vector<double>& dist, std::vector<bool>& visited) {
        double min = INT_MAX; int min_index;

        for (int v = 0; v < num_vertex; ++v) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                min_index = v;
            }
        }

        return min_index;
    }

    bool edge_exists(int u, int v) {
        return adj_matrix[u][v];
    }

    void DFS_util(int vertex, std::vector<bool>& visited, bool benchmark) {
        visited[vertex] = true;
        if (!benchmark) std::cout << vertex << " ";

        for (int i = 0; i < num_vertex; ++i) {
            if (adj_matrix[vertex][i] != 0 && !visited[i]) {
                DFS_util(i, visited, benchmark);
            }
        }
    }

    void dfs_min_weight_helper(std::vector<bool>& visited, int v, int prev_vertex, double prev_weight, bool benchmark = false) {
        visited[v] = true;
        if (!benchmark) std::cout << v << " ";

        std::vector<std::pair<int, double>> sorted_neighbors;
        for (int i = 0; i < num_vertex; ++i) {
            if (adj_matrix[v][i] != 0 && !visited[i]) {
                sorted_neighbors.push_back({ i, adj_matrix[v][i] });
            }
        }
        std::sort(sorted_neighbors.begin(), sorted_neighbors.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second < b.second; 
            });

        for (const auto& neighbor : sorted_neighbors) {
            if (!visited[neighbor.first]) {
                dfs_min_weight_helper(visited, neighbor.first, v, neighbor.second, benchmark);
            }
        }
    }

    void dfs_cycle(std::vector<int>& visited, int v, bool& has_cycle) {
        visited[v] = 1;

        for (int i = 0; i < num_vertex; ++i) {
            if (adj_matrix[v][i] != 0) {
                if (visited[i] == 0) {
                    dfs_cycle(visited, i, has_cycle);
                }
                else if (visited[i] == 1) {
                    has_cycle = true;
                }
            }
        }
        visited[v] = 2;
    }

    void dfs_connected(int v, std::vector<bool>& visited) {
        visited[v] = true;
        for (int i = 0; i < num_vertex; ++i) {
            if (adj_matrix[v][i] != 0 && !visited[i]) {
                dfs_connected(i, visited);
            }
        }
    }

    void dfs_connected_components(int v, std::vector<bool>& visited, std::vector<int>& component) {
        visited[v] = true;
        component.push_back(v);
        for (int i = 0; i < num_vertex; ++i) {
            if (adj_matrix[v][i] != 0 && !visited[i]) {
                dfs_connected_components(i, visited, component);
            }
        }
    }

    void topological_sort_util(int v, std::vector<bool>& visited, std::stack<int>& Stack) {
        visited[v] = true;
        for (int i = 0; i < num_vertex; ++i) {
            if (adj_matrix[v][i] && !visited[i])
                topological_sort_util(i, visited, Stack);
        }
        Stack.push(v);
    }

    void build_spanning_forest_dfs(int start, std::vector<bool>& visited, adj_matrix_graph& forest) {
        visited[start] = true;
        for (int v = 0; v < num_vertex; ++v) {
            if (adj_matrix[start][v] != 0 && !visited[v]) {
                forest.add_edge(start, v, adj_matrix[start][v]);
                build_spanning_forest_dfs(v, visited, forest);
            }
        }
    }

    int find(std::vector<int>& parent, int vertex) {
        if (parent[vertex] != vertex)
            parent[vertex] = find(parent, parent[vertex]);
        return parent[vertex];
    }

    void union_sets(std::vector<int>& parent, int x, int y) {
        int rootX = find(parent, x);
        int rootY = find(parent, y);
        parent[rootX] = rootY;
    }

    std::vector<edge> get_edges() {
        std::vector<edge> edges;
        for (int i = 0; i < num_vertex; ++i) {
            for (int j = 0; j < num_vertex; ++j) {
                if (adj_matrix[i][j] != 0) {
                    edge ed(i, j, adj_matrix[i][j]);
                    edges.push_back(ed);
                }
            }
        }

        std::sort(edges.begin(), edges.end(), [](const edge& a, const edge& b) {
            return a.dis < b.dis;
            });

        return edges;
    }
};

adj_matrix_graph convert_list_to_matrix(const adj_list_graph& graph) {
    adj_matrix_graph result;
    result.num_vertex = graph.num_vertex;
    result.adj_matrix.resize(graph.num_vertex, std::vector<double>(graph.num_vertex, 0));

    for (int i = 0; i < graph.num_vertex; ++i) {
        for (const auto& edge : graph.adj_list[i]) {
            int dest_vertex = edge.first;
            double weight = edge.second;
            result.adj_matrix[i][dest_vertex] = weight;
            if (!graph.directed) {
                result.adj_matrix[dest_vertex][i] = weight; 
            }
        }
    }

    result.directed = graph.directed;
    return result;
}

adj_list_graph convert_matrix_to_list(const adj_matrix_graph& graph) {
    adj_list_graph result;
    result.num_vertex = graph.num_vertex;
    result.adj_list = new std::list<std::pair<int, double>>[graph.num_vertex];

    for (int i = 0; i < graph.num_vertex; ++i) {
        for (int j = 0; j < graph.num_vertex; ++j) {
            if (graph.adj_matrix[i][j] != 0) {
                result.adj_list[i].push_back({ j, graph.adj_matrix[i][j] });
                if (!graph.directed && i != j) {
                    result.adj_list[j].push_back({ i, graph.adj_matrix[i][j] }); 
                }
            }
        }
    }

    result.directed = graph.directed;
    return result;
}

void dem_mode();


void benchmark();
void benchmark_undir_matrix();
void benchmark_dir_matrix();
void benchmark_undir_list();
void benchmark_dir_list();

void interactive_mode();
void interactive_mode_list();
void interactive_mode_matrix();

void clear_console();

int main()
{
    srand(time(nullptr));
    bool exit = false;
    while (!exit) {
        std::cout << "Choose mode:\n"
            "1. Interactive mode;\n"
            "2. Demonstration mode;\n"
            "3. Benchmark mode;\n"
            "4. Exit.\n";
        int mode;
        std::cin >> mode;
        switch (mode) {
        case 1:
            break;
        case 2:
            dem_mode();
            break;
        case 3:
            benchmark();
            break;
        case 4:
            exit = true;
            break;
        default:
            std::cout << "Wrong mode!\n";
            break;
        }
    }
}
void interactive_mode() {

    std::cout << "Choose type of graph:\n"
        "1. Adjacency list\n"
        "2. Adjacency matrix\n"
        "3. Exit\n";
        int mode;
        std::cin >> mode;
        switch (mode) {
        case 1:
            std::cout << "Adjacency list\n";
            interactive_mode_list();
            break;
        case 2:
            std::cout << "Adjacency matrix\n";
            interactive_mode_matrix();
            break;
        default:
            std::cout << "Wrong mode!\n";
            break;
        }
}

void interactive_mode_list() {
    int choice;
    bool exit = false;
    int numVertex;
    std::cout << "Enter num of vertex: ";
    std::cin >> numVertex;
    bool directed;
    std::cout << "Is directed graphs? (Y or y - yes): ";
    char a; std::cin >> a;
    directed = (a == 'Y' || a == 'y') ? true : false;
    adj_list_graph graph(numVertex, directed);
    adj_list_graph spanning_forest;

    while (!exit) {
        std::cout << "\n--- Menu ---" << std::endl;
        std::cout << "1. Add Edge" << std::endl;
        std::cout << "2. Remove Edge" << std::endl;
        std::cout << "3. Print Graph" << std::endl;
        std::cout << "4. Check Connectivity" << std::endl;
        std::cout << "5. Find Connected Components" << std::endl;
        std::cout << "6. Check Acyclicity" << std::endl;
        std::cout << "7. Depth-First Search" << std::endl;
        std::cout << "8. Depth-First Search for Minimum Weight" << std::endl;
        std::cout << "9. Dijkstra's Algorithm" << std::endl;
        std::cout << "10. Topological Sort" << std::endl;
        std::cout << "11. Build Spanning Forest" << std::endl;
        std::cout << "12. Kruskal's Algorithm" << std::endl;
        std::cout << "13. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            int u, v;
            double weight;
            std::cout << "Enter source vertex: ";
            std::cin >> u;
            std::cout << "Enter destination vertex: ";
            std::cin >> v;
            std::cout << "Enter weight: ";
            std::cin >> weight;
            graph.add_edge(u, v, weight);
            break;
        }
        case 2: {
            int u, v;
            std::cout << "Enter source vertex: ";
            std::cin >> u;
            std::cout << "Enter destination vertex: ";
            std::cin >> v;
            graph.remove_edge(u, v);
            break;
        }
        case 3: {
            graph.print_graph();
            break;
        }
        case 4: {
            bool connected = graph.is_connected();
            std::cout << (connected ? "Graph is connected." : "Graph is not connected.") << std::endl;
            break;
        }
        case 5: {
            graph.find_connected_components();
            break;
        }
        case 6: {
            bool acyclic = graph.is_acyclic();
            std::cout << (acyclic ? "Graph is acyclic." : "Graph has cycles.") << std::endl;
            break;
        }
        case 7: {
            graph.DFS();
            break;
        }
        case 8: {
            graph.dfs_min_weight();
            break;
        }
        case 9: {
            int source;
            std::cout << "Enter source vertex for Dijkstra's Algorithm: ";
            std::cin >> source;
            graph.dijkstra(source);
            break;
        }
        case 10: {
            graph.topological_sort();
            break;
        }
        case 11: {
            graph.build_spanning_forest(spanning_forest);
            std::cout << "Spanning Forest created." << std::endl;
            spanning_forest.print_graph();
            break;
        }
        case 12: {
            graph.kruskal();
            break;
        }
        case 13: {
            exit = true;
            break;
        }
        default: {
            std::cout << "Invalid choice. Please enter a number between 1 and 14." << std::endl;
            break;
        }
        }
    }


}
void interactive_mode_matrix() {
    int choice;
    bool exit = false;
    int numVertex;
    std::cout << "Enter num of vertex: ";
    std::cin >> numVertex;
    bool directed;
    std::cout << "Is directed graphs? (Y or y - yes): ";
    char a; std::cin >> a;
    directed = (a == 'Y' || a == 'y') ? true : false;
    adj_matrix_graph graph(numVertex, directed);
    adj_matrix_graph spanning_forest;

    int choice;
    do {
        std::cout << "\n--- Menu ---" << std::endl;
        std::cout << "1. Add edge" << std::endl;
        std::cout << "2. Remove edge" << std::endl;
        std::cout << "3. Print graph" << std::endl;
        std::cout << "4. Create random graph" << std::endl;
        std::cout << "5. Check if connected" << std::endl;
        std::cout << "6. Find connected components" << std::endl;
        std::cout << "7. Check if acyclic" << std::endl;
        std::cout << "8. Depth-first search (DFS)" << std::endl;
        std::cout << "9. DFS for minimum weight" << std::endl;
        std::cout << "10. Floyd-Warshall algorithm" << std::endl;
        std::cout << "11. Dijkstra's algorithm" << std::endl;
        std::cout << "12. Topological sort" << std::endl;
        std::cout << "13. Build spanning forest" << std::endl;
        std::cout << "14. Kruskal's algorithm" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            int u, v;
            double weight;
            std::cout << "Enter source vertex: ";
            std::cin >> u;
            std::cout << "Enter destination vertex: ";
            std::cin >> v;
            std::cout << "Enter weight: ";
            std::cin >> weight;
            graph.add_edge(u, v, weight);
            break;
        }
        case 2: {
            int u, v;
            std::cout << "Enter source vertex: ";
            std::cin >> u;
            std::cout << "Enter destination vertex: ";
            std::cin >> v;
            graph.remove_edge(u, v);
            break;
        }
        case 3:
            graph.print_graph();
            break;
        case 4: {
            int n, max_weight;
            std::cout << "Enter the number of edges: ";
            std::cin >> n;
            std::cout << "Enter the maximum weight: ";
            std::cin >> max_weight;
            graph.create_random_graph(n, max_weight);
            break;
        }
        case 5:
            std::cout << "Graph is " << (graph.is_connected() ? "" : "not ") << "connected." << std::endl;
            break;
        case 6:
            graph.find_connected_components();
            break;
        case 7:
            std::cout << "Graph is " << (graph.is_acyclic() ? "" : "not ") << "acyclic." << std::endl;
            break;
        case 8:
            std::cout << "Depth-first search (DFS) starting from vertex 0:" << std::endl;
            graph.DFS();
            std::cout << std::endl;
            break;
        case 9:
            std::cout << "DFS for minimum weight starting from vertex 0:" << std::endl;
            graph.dfs_min_weight();
            std::cout << std::endl;
            break;
        case 10:
            std::cout << "Applying Floyd-Warshall algorithm:" << std::endl;
            graph.al_floyde();
            break;
        case 11: {
            int start_vertex;
            std::cout << "Enter the starting vertex: ";
            std::cin >> start_vertex;
            graph.dijkstra(start_vertex);
            break;
        }
        case 12:
            std::cout << "Topological sorting:" << std::endl;
            graph.topological_sort();
            std::cout << std::endl;
            break;
        case 13: {
            std::cout << "Building spanning forest:" << std::endl;
            adj_matrix_graph forest = graph.build_spanning_forest();
            std::cout << "Spanning forest: " << std::endl;
            forest.print_graph();
            break;
        }
        case 14:
            std::cout << "Applying Kruskal's algorithm:" << std::endl;
            graph.kruskal();
            break;
        case 0:
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);
}
void clear_console() {
    std::cout << "Enter any symbol and press enter to clear the console...";
    char k;
    std::cin >> k;
    std::cin.ignore(1000, '\n');
    std::system("clear || cls");
}

void dem_mode() {
    std::cout << "Demonstration mode\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Create 4 random graph with 6 verticies and 10 edges: \n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "First graph (adj_list + directed): \n";
    adj_list_graph list_graph_dir(6, true); 
    list_graph_dir.create_random_graph(10, 10);
    list_graph_dir.print_graph();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Second graph(adj_list + undirected): \n";
    adj_list_graph list_graph_undir(6, false);
    list_graph_undir.create_random_graph(10, 10);
    list_graph_undir.print_graph();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Third graph(adj_matrix + directed): \n";
    adj_matrix_graph matrix_graph_dir(6, true);
    matrix_graph_dir.create_random_graph(10, 10);
    matrix_graph_dir.print_graph();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Fourth graph(adj_matrix + undirected): \n";
    adj_matrix_graph matrix_graph_undir(6, false);
    matrix_graph_undir.create_random_graph(10, 10);
    matrix_graph_undir.print_graph();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Lets check connected undirected graph and findLet's check the connectivity of directed graphs and find their connectivity components\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Second graph: \n";
    std::cout << "Is connected: " << (list_graph_undir.is_connected() ? "Yes\n" : "No\n");
    list_graph_undir.find_connected_components();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Fourth graph: \n";
    std::cout << "Is connected: " << (matrix_graph_undir.is_connected() ? "Yes\n" : "No\n");
    matrix_graph_undir.find_connected_components();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let's check the oriented graph for acyclicity\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "First graph: \n";
    std::cout << "Is acyclic: " << (list_graph_dir.is_acyclic() ? "Yes\n" : "No\n");
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Third graph: \n";
    std::cout << "Is acyclic: " << (matrix_graph_dir.is_acyclic() ? "Yes\n" : "No\n");
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let's go through all the graphs in depth. First in random order, then by edge weight\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "First graph (adj_list + directed): \n";
    std::cout << "Random order: ";  list_graph_dir.DFS(); std::cout << std::endl;
    std::cout << "By edge weight order: ";  list_graph_dir.dfs_min_weight(); std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout << "Second graph(adj_list + undirected): \n";
    std::cout << "Random order: "; list_graph_undir.DFS();; std::cout << std::endl;
    std::cout << "By edge weight order: "; list_graph_undir.dfs_min_weight(); std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Third graph(adj_matrix + directed): \n";
    std::cout << "Random order: "; matrix_graph_dir.DFS();; std::cout << std::endl;
    std::cout << "By edge weight order: "; matrix_graph_dir.dfs_min_weight(); std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Fourth graph(adj_matrix + undirected): \n";
    std::cout << "Random order: "; matrix_graph_undir.DFS(); std::cout << std::endl;
    std::cout << "By edge weight order: "; matrix_graph_undir.dfs_min_weight(); std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let's apply Dijkstra's algorithm for 4 vertices of all graphs\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "First graph (adj_list + directed): \n";
    list_graph_dir.dijkstra(4);
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Second graph(adj_list + undirected): \n";
    list_graph_undir.dijkstra(4);
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Third graph(adj_matrix + directed): \n";
    matrix_graph_dir.dijkstra(4);
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Fourth graph(adj_matrix + undirected): \n";
    matrix_graph_undir.dijkstra(4);
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let's apply Floyd's algorithm for all adj_matrix graphs\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Third graph(adj_matrix + directed): \n";
    matrix_graph_dir.al_floyde();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Fourth graph(adj_matrix + undirected): \n";
    matrix_graph_undir.al_floyde();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let's apply topological sorting for undirected graphs\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "First graph: \n";
    std::cout << "Topological sort: ";  list_graph_dir.topological_sort(); std::cout << std::endl;
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Third graph: \n";
    std::cout << "Topological sort: "; matrix_graph_dir.topological_sort(); std::cout << std::endl;
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let's create a spanning tree using the depth-first search method\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Second graph(adj_list + undirected): \n";
    adj_list_graph list_span_graph;
    list_graph_undir.build_spanning_forest(list_span_graph);
    list_span_graph.print_graph();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Fourth graph(adj_matrix + undirected): \n";
    adj_matrix_graph matrix_span_graph = matrix_graph_undir.build_spanning_forest();
    matrix_span_graph.print_graph();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let's create a min spanning tree using the Kruskal algorithm\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Second graph(adj_list + undirected): \n";
    list_graph_undir.kruskal();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Fourth graph(adj_matrix + undirected): \n";
    matrix_graph_undir.kruskal();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

}


void benchmark_undir_matrix() {
    std::cout << "Benchmark for undirected graph (adjacency matrix)\n\n";
    adj_matrix_graph graph(bm_ver);

    auto start_cg = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Create random graph");
        graph.create_random_graph(bm_ed, bm_mw);
    }
    auto end_cg = std::chrono::high_resolution_clock::now();
    auto duration_cg = std::chrono::duration_cast<std::chrono::milliseconds>(end_cg- start_cg);
    std::cout << "Time taken by create graph function: " << duration_cg.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_ic = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Check connected");
        graph.is_connected();
    }
    auto end_ic = std::chrono::high_resolution_clock::now();
    auto duration_ic = std::chrono::duration_cast<std::chrono::milliseconds>(end_ic - start_ic);
    std::cout << "Time taken by is_concected function: " << duration_ic.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_fcc = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Find connected components");
        graph.find_connected_components(bm);
    }

    auto end_fcc = std::chrono::high_resolution_clock::now();
    auto duration_fcc = std::chrono::duration_cast<std::chrono::milliseconds>(end_fcc - start_fcc);
    std::cout << "Time taken by Find connected components function: " << duration_fcc.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dfs = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("DFS");
        graph.DFS(0, bm);
    }
    auto end_dfs = std::chrono::high_resolution_clock::now();
    auto duration_dfs = std::chrono::duration_cast<std::chrono::milliseconds>(end_dfs - start_dfs);
    std::cout << "Time taken by DFS function: " << duration_dfs.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dfsmin = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("DFS min meight edge");
        graph.dfs_min_weight(bm);
    }
    auto end_dfsmin = std::chrono::high_resolution_clock::now();
    auto duration_dfsmin = std::chrono::duration_cast<std::chrono::milliseconds>(end_dfsmin - start_dfsmin);
    std::cout << "Time taken by DFS min meight edge function: " << duration_dfs.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_af = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Floyd alogrorithm");
        graph.al_floyde(bm);
    }
    auto end_af = std::chrono::high_resolution_clock::now();
    auto duration_af = std::chrono::duration_cast<std::chrono::milliseconds>(end_af - start_af);
    std::cout << "Time taken by Floyde algorithm: " << duration_af.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dj = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Dijkstra alogrorithm");
        graph.dijkstra(0, bm);
    }
    auto end_dj = std::chrono::high_resolution_clock::now();
    auto duration_dj = std::chrono::duration_cast<std::chrono::milliseconds>(end_dj - start_dj);
    std::cout << "Time taken by Dijkstra algorithm: " << duration_dj.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_st = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Build spanning forest");
        adj_matrix_graph spanning_tree = graph.build_spanning_forest();
    }
    auto end_st = std::chrono::high_resolution_clock::now();
    auto duration_st = std::chrono::duration_cast<std::chrono::milliseconds>(end_st - start_st);
    std::cout << "Time taken by build_spanning_forest function: " << duration_st.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_kr = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Kruskal algorithm");
        graph.kruskal(bm);
    }
    auto end_kr = std::chrono::high_resolution_clock::now();
    auto duration_kr = std::chrono::duration_cast<std::chrono::milliseconds>(end_kr - start_kr);
    std::cout << "Time taken by Kruskal algorithm: " << duration_kr.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
void benchmark_dir_matrix() {
    std::cout << "Benchmark for directed graph (adjacency matrix)\n\n";
    adj_matrix_graph dir_graph(bm_ver, true);

    auto start_cg = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Create random graph");
        dir_graph.create_random_graph(bm_ed, bm_mw);
    }
    auto end_cg = std::chrono::high_resolution_clock::now();
    auto duration_cg = std::chrono::duration_cast<std::chrono::milliseconds>(end_cg - start_cg);
    std::cout << "Time taken by create graph function: " << duration_cg.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_ia = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Check acyclic");
        dir_graph.is_acyclic();
    }
    auto end_ia = std::chrono::high_resolution_clock::now();
    auto duration_ia = std::chrono::duration_cast<std::chrono::milliseconds>(end_ia - start_ia);
    std::cout << "Time taken by check acyclic function: " << duration_ia.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dfs = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("DFS");
        dir_graph.DFS(0, bm);
    }
    auto end_dfs = std::chrono::high_resolution_clock::now();
    auto duration_dfs = std::chrono::duration_cast<std::chrono::milliseconds>(end_dfs - start_dfs);
    std::cout << "Time taken by DFS function: " << duration_dfs.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dfsmin = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("DFS min meight edge");
        dir_graph.dfs_min_weight(bm);
    }
    auto end_dfsmin = std::chrono::high_resolution_clock::now();
    auto duration_dfsmin = std::chrono::duration_cast<std::chrono::milliseconds>(end_dfsmin - start_dfsmin);
    std::cout << "Time taken by DFS min meight edge function: " << duration_dfs.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_af = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Floyd alogrorithm");
        dir_graph.al_floyde(bm);
    }
    auto end_af = std::chrono::high_resolution_clock::now();
    auto duration_af = std::chrono::duration_cast<std::chrono::milliseconds>(end_af - start_af);
    std::cout << "Time taken by Floyde algorithm: " << duration_af.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dj = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Dijkstra alogrorithm");
        dir_graph.dijkstra(0, bm);
    }
    auto end_dj = std::chrono::high_resolution_clock::now();
    auto duration_dj = std::chrono::duration_cast<std::chrono::milliseconds>(end_dj - start_dj);
    std::cout << "Time taken by Dijkstra algorithm: " << duration_dj.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_ts = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Topological sort");
        dir_graph.topological_sort(bm);
    }
    auto end_ts = std::chrono::high_resolution_clock::now();
    auto duration_ts = std::chrono::duration_cast<std::chrono::milliseconds>(end_ts - start_ts);
    std::cout << "Time taken by topological sort alogrithm: " << duration_ts.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
void benchmark_undir_list() {
    std::cout << "Benchmark for undirected graph (adjacency list)\n\n";
    adj_list_graph graph(bm_ver);

    auto start_cg = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Create random graph");
        graph.create_random_graph(bm_ed, bm_mw);
    }
    auto end_cg = std::chrono::high_resolution_clock::now();
    auto duration_cg = std::chrono::duration_cast<std::chrono::milliseconds>(end_cg - start_cg);
    std::cout << "Time taken by create graph function: " << duration_cg.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_ic = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Check connected");
        graph.is_connected();
    }
    auto end_ic = std::chrono::high_resolution_clock::now();
    auto duration_ic = std::chrono::duration_cast<std::chrono::milliseconds>(end_ic - start_ic);
    std::cout << "Time taken by is_concected function: " << duration_ic.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_fcc = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Find connected components");
        graph.find_connected_components(bm);
    }

    auto end_fcc = std::chrono::high_resolution_clock::now();
    auto duration_fcc = std::chrono::duration_cast<std::chrono::milliseconds>(end_fcc - start_fcc);
    std::cout << "Time taken by Find connected components function: " << duration_fcc.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dfs = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("DFS");
        graph.DFS(0, bm);
    }
    auto end_dfs = std::chrono::high_resolution_clock::now();
    auto duration_dfs = std::chrono::duration_cast<std::chrono::milliseconds>(end_dfs - start_dfs);
    std::cout << "Time taken by DFS function: " << duration_dfs.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dfsmin = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("DFS min meight edge");
        graph.dfs_min_weight(bm);
    }
    auto end_dfsmin = std::chrono::high_resolution_clock::now();
    auto duration_dfsmin = std::chrono::duration_cast<std::chrono::milliseconds>(end_dfsmin - start_dfsmin);
    std::cout << "Time taken by DFS min meight edge function: " << duration_dfs.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dj = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Dijkstra alogrorithm");
        graph.dijkstra(0, bm);
    }
    auto end_dj = std::chrono::high_resolution_clock::now();
    auto duration_dj = std::chrono::duration_cast<std::chrono::milliseconds>(end_dj - start_dj);
    std::cout << "Time taken by Dijkstra algorithm: " << duration_dj.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_st = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Build spanning forest");
        adj_list_graph spaning_tree;
        graph.build_spanning_forest(spaning_tree);
    }
    auto end_st = std::chrono::high_resolution_clock::now();
    auto duration_st = std::chrono::duration_cast<std::chrono::milliseconds>(end_st - start_st);
    std::cout << "Time taken by build_spanning_forest function: " << duration_st.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_kr = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Kruskal algorithm");
        graph.kruskal(bm);
    }
    auto end_kr = std::chrono::high_resolution_clock::now();
    auto duration_kr = std::chrono::duration_cast<std::chrono::milliseconds>(end_kr - start_kr);
    std::cout << "Time taken by Kruskal algorithm: " << duration_kr.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
void benchmark_dir_list() {
    std::cout << "Benchmark for directed graph (adjacency matrix)\n\n";
    adj_list_graph dir_graph(bm_ver, true);

    auto start_cg = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Create random graph");
        dir_graph.create_random_graph(bm_ed, bm_mw);
    }
    auto end_cg = std::chrono::high_resolution_clock::now();
    auto duration_cg = std::chrono::duration_cast<std::chrono::milliseconds>(end_cg - start_cg);
    std::cout << "Time taken by create graph function: " << duration_cg.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_ia = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Check acyclich");
        dir_graph.is_acyclic();
    }
    auto end_ia = std::chrono::high_resolution_clock::now();
    auto duration_ia = std::chrono::duration_cast<std::chrono::milliseconds>(end_ia - start_ia);
    std::cout << "Time taken by check acyclic function: " << duration_ia.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dfs = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("DFS");
        dir_graph.DFS(0, bm);
    }
    auto end_dfs = std::chrono::high_resolution_clock::now();
    auto duration_dfs = std::chrono::duration_cast<std::chrono::milliseconds>(end_dfs - start_dfs);
    std::cout << "Time taken by DFS function: " << duration_dfs.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dfsmin = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("DFS min meight edge");
        dir_graph.dfs_min_weight(bm);
    }
    auto end_dfsmin = std::chrono::high_resolution_clock::now();
    auto duration_dfsmin = std::chrono::duration_cast<std::chrono::milliseconds>(end_dfsmin - start_dfsmin);
    std::cout << "Time taken by DFS min meight edge function: " << duration_dfs.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_dj = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Dijkstra alogrorithm");
        dir_graph.dijkstra(0, bm);
    }
    auto end_dj = std::chrono::high_resolution_clock::now();
    auto duration_dj = std::chrono::duration_cast<std::chrono::milliseconds>(end_dj - start_dj);
    std::cout << "Time taken by Dijkstra algorithm: " << duration_dj.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start_ts = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Topological sort");
        dir_graph.topological_sort(bm);
    }
    auto end_ts = std::chrono::high_resolution_clock::now();
    auto duration_ts = std::chrono::duration_cast<std::chrono::milliseconds>(end_ts - start_ts);
    std::cout << "Time taken by topological sort alogrithm: " << duration_ts.count() << " milliseconds" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void benchmark() {
    benchmark_undir_matrix();
    clear_console();
    benchmark_dir_matrix();
    clear_console();
    benchmark_undir_list();
    clear_console();
    benchmark_dir_list();
}
