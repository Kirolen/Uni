// Block 0: 1, 4
// Block 1:
#include <iostream>
#include <vector>
#include <iomanip> 
#include <cstdlib> 
#include <ctime>   

class graph_adj_matrix {
public:
    std::vector<std::vector<double>> adj_matrix;
    size_t num_vertices;
    bool directed;

    graph_adj_matrix(size_t num_vertices = 0, bool directed = true) : num_vertices(num_vertices), directed(directed) {
        adj_matrix.resize(num_vertices, std::vector<double>(num_vertices, 0.0));
    }

    void add_vertex() {
        for (size_t i = 0; i < num_vertices; ++i) {
            adj_matrix[i].push_back(0.0); 
        }
        std::vector<double> new_row(num_vertices + 1, 0.0); 
        adj_matrix.push_back(new_row); 
        ++num_vertices; 
    }

    void add_edge(size_t from, size_t to, double weight) {
        if (from >= 0 && from < num_vertices && to >= 0 && to < num_vertices) {
            adj_matrix[from][to] = weight;
            if (directed) adj_matrix[to][from] = weight;
        }
        else {
            std::cerr << "Invalid vertex index\n";
        }
    }

    void remove_edge(size_t from, size_t to) {
        if (from >= 0 && from < num_vertices && to >= 0 && to < num_vertices) {
            adj_matrix[from][to] = 0;
            if (!directed) adj_matrix[to][from] = 0;
        }
        else {
            std::cerr << "Invalid vertex index\n";
        }
    }

    void print_graph() const {
        std::cout << "Graph adjacency matrix:\n";
        for (size_t i = 0; i < num_vertices; ++i) {
            for (size_t j = 0; j < num_vertices; ++j) {
                std::cout << std::setw(5) << adj_matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void generate_random_graph(size_t num_edges) {
        if (num_vertices < 1) return;
        int count_edges = 0;
        while (count_edges < num_edges) {
            size_t from = rand() % num_vertices;
            size_t to = rand() % num_vertices;
            double weight = rand() % 10 + 1;
            if (from != to && adj_matrix[from][to] == 0) {
                add_edge(from, to, weight);
                directed ? count_edges += 2 : count_edges++;
            }
        }
    }
};

class graph_bit_vector {
private:
    bool get_bit(unsigned int bit_vector, unsigned int index) {
        return (bit_vector >> index) & 1;
    }

    void set_bit(unsigned int& bit_vector, unsigned int index, bool value) {
        unsigned int mask = 1 << index;
        if (value) {
            bit_vector |= mask;
        }
        else {
            mask = ~mask;
            bit_vector &= mask;
        }
    }

public:
    std::vector<std::vector<double>> matrix_rows;
    std::size_t num_vertices;
    bool directed;

    graph_bit_vector(std::size_t num_vertices = 0, bool directed = true) : num_vertices(num_vertices), directed(directed) {
        matrix_rows.resize(num_vertices, std::vector<double>(num_vertices, 0)); // Initialize matrix with all zeros
        srand(time(0)); // Seed random number generator
    }

    void add_edge(std::size_t from, std::size_t to, double weight = 1.0) { // Added support for weighted edges
        if (from >= num_vertices || from < 0 || to >= num_vertices || to < 0) {
            std::cerr << "Invalid vertex index\n";
            return;
        }
        matrix_rows[from][to] = weight; // Set the weight of the edge
        if (!directed) matrix_rows[to][from] = weight; // If undirected, set weight for the reverse edge as well
    }

    void remove_edge(std::size_t from, std::size_t to) {
        if (from >= num_vertices || from < 0 || to >= num_vertices || to < 0) {
            std::cerr << "Invalid vertex index\n";
            return;
        }
        matrix_rows[from][to] = 0; // Remove the edge by setting weight to 0
        if (!directed) matrix_rows[to][from] = 0; // If undirected, remove the reverse edge as well
    }

    void print_matrix() {
        for (std::size_t i = 0; i < num_vertices; i++) {
            for (std::size_t j = 0; j < num_vertices; j++) {
                std::cout << std::setw(6) << matrix_rows[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void print_edges() {
        for (std::size_t i = 0; i < num_vertices; i++) {
            for (std::size_t j = 0; j < num_vertices; j++) {
                if (matrix_rows[i][j] != 0) {
                    std::cout << i << "->" << j << " (Weight: " << matrix_rows[i][j] << ")" << std::endl;
                }
            }
        }
    }

    void generate_random_graph(size_t num_edges, int max_weight) {
        if (num_vertices < 1) return;

        for (size_t count_edges = 0; count_edges < num_edges; directed ? count_edges +=2 : count_edges++) {
            size_t from = rand() % num_vertices;
            size_t to = rand() % num_vertices;
            int weight = rand() % (max_weight + 1); 

            if (matrix_rows[from][to] == 0) add_edge(from, to, weight);
        }
    }
};

void convert_graph_from_adj_to_bit_vector(const graph_adj_matrix& adj_matrix, graph_bit_vector& bit_vector) {
    bit_vector = graph_bit_vector(adj_matrix.num_vertices, adj_matrix.directed);

    for (size_t i = 0; i < adj_matrix.num_vertices; ++i) {
        for (size_t j = 0; j < adj_matrix.num_vertices; ++j) {
            double weight = adj_matrix.adj_matrix[i][j];
            if (weight != 0) {
                bit_vector.add_edge(i, j, weight);
            }
        }
    }
}

void convert_graph_from_bit_to_adj_matrix(const graph_bit_vector& bit_vector, graph_adj_matrix& adj_matrix) {
    adj_matrix = graph_adj_matrix(bit_vector.num_vertices, bit_vector.directed);

    for (size_t i = 0; i < bit_vector.num_vertices; ++i) {
        for (size_t j = 0; j < bit_vector.num_vertices; ++j) {
            double weight = bit_vector.matrix_rows[i][j];
            if (weight != 0) {
                adj_matrix.add_edge(i, j, weight);
            }
        }
    }
}


void demonstration_mode();

int main() {
    srand(time(NULL)); 
    int num_vertices = 5;
    graph_bit_vector graph(num_vertices);
    graph.generate_random_graph(10, 10);
    graph.print_matrix();
    graph.print_edges();
    graph_adj_matrix h;
    convert_graph_from_bit_to_adj_matrix(graph, h);
    h.print_graph();

    //demonstration_mode();
    return 0;
}

void demonstration_mode() {
    int num_vertices = 5;

    std::cout << "Demonstration Mode\n";
    std::cout << "Adjacent Matrix Graph\n";
    for (int mode = 0; mode < 2; mode++) {
        graph_adj_matrix adj_matrix_graph(num_vertices, mode == 1 ? false : true);
        mode == 1 ? std::cout << "Initial Directed Graph:\n" : std::cout << "Initial Undirected Graph:\n";
        adj_matrix_graph.print_graph();
        std::cout << "Adding a vertex:\n";
        adj_matrix_graph.add_vertex();
        adj_matrix_graph.print_graph();
        std::cout << "Adding edges from 0 to 2, 1 to 4 with weight 3:\n";
        adj_matrix_graph.add_edge(0, 2, 3);
        adj_matrix_graph.add_edge(0, 1, 4);
        adj_matrix_graph.print_graph();
        std::cout << "Removing edges from 1 to 4:\n";
        adj_matrix_graph.remove_edge(1, 4);
        adj_matrix_graph.print_graph();
        std::cout << "Generating a random graph with 7 edges:\n";
        adj_matrix_graph.generate_random_graph(7);
        adj_matrix_graph.print_graph();
        std::cout << std::endl << std::endl << std::endl << std::endl;
    }

    std::cout << "Bit Vector Graph\n";
    for (int mode = 0; mode < 2; mode++) {
        graph_bit_vector bit_vector_graph(5, mode == 1 ? false : true);
        mode == 1 ? std::cout << "Initial Directed Graph:\n" : std::cout << "Initial Undirected Graph:\n";
        std::cout << "Initial Bit Vector Graph\n";
        std::cout << "Print edges:\n";
        bit_vector_graph.print_edges();
        std::cout << "Print matrix:\n";
        bit_vector_graph.print_matrix();
        std::cout << "Adding an edge from 4 to 2, 3 to 1 with weight 3:\n";
        bit_vector_graph.add_edge(4, 2, 3);
        bit_vector_graph.add_edge(3, 1, 3);
        std::cout << "Removing edges from 4 to 2:\n";
        bit_vector_graph.remove_edge(4, 2);
        std::cout << "Print edges:\n";
        bit_vector_graph.print_edges();
        std::cout << "Print matrix:\n";
        bit_vector_graph.print_matrix();
        std::cout << "Generating a random graph with 7 edges:\n";
        bit_vector_graph.generate_random_graph(20, 10);
        std::cout << "Print edges:\n";
        bit_vector_graph.print_edges();
        std::cout << "Print matrix:\n";
        bit_vector_graph.print_matrix();
        std::cout << std::endl << std::endl << std::endl << std::endl;
    }
}
