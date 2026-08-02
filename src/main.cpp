#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

constexpr int window_width = 600;
constexpr int window_height = 600;

constexpr int rows = 200;
constexpr int columns = 200;

constexpr float cell_width = static_cast<float>(window_width) / columns;
constexpr float cell_height = static_cast<float>(window_height) / rows;

struct Cell {
    double a = 1.0;
    double b = 0.0;
};

constexpr double diffusion_a = 0.2;
constexpr double diffusion_b = 0.1;
constexpr double feed_rate = 0.055;
constexpr double kill_rate = 0.062;

const int center_row = rows / 2;
const int center_column = columns / 2;
const int center_index = center_row * columns + center_column;

// Defines seeded chemical values.
void ResetSimulation(std::vector<Cell>& current_grid,
                     std::vector<Cell>& next_grid) {
    current_grid.assign(rows * columns, Cell{});
    next_grid.assign(rows * columns, Cell{});

    for (int row = center_row - 3; row <= center_row + 3; ++row) {
        for (int column = center_column - 3; column <= center_column + 3;
             ++column) {
            const int index = row * columns + column;

            current_grid[index].a = 0.0;
            current_grid[index].b = 1.0;
        }
    }
}

template <typename Callable>
void ForEachCell(Callable callable) {
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            callable(row, column);
        }
    }
}

std::vector<const Cell*> GetNeighbors(const std::vector<Cell>& current_grid,
                                      int index) {
    const int row = index / columns;
    const int column = index % columns;
    std::vector<const Cell*> neighbors;

    if (row != 0) {
        neighbors.push_back(&current_grid[index - columns]);
    }
    if (column != columns - 1) {
        neighbors.push_back(&current_grid[index + 1]);
    }
    if (row != rows - 1) {
        neighbors.push_back(&current_grid[index + columns]);
    }
    if (column != 0) {
        neighbors.push_back(&current_grid[index - 1]);
    }
    return neighbors;
}

// Calculates the concentration difference with neighboring cells.
// Positive: neighbors have more. Negative: current cell has more.
double CalculateLaplacian(const std::vector<Cell>& current_grid, int index,
                          double Cell::* value) {
    const Cell& current_cell = current_grid[index];
    std::vector<const Cell*> neighbors = GetNeighbors(current_grid, index);
    double sum_neighbors = 0.0;
    for (auto* neighbor : neighbors) {
        sum_neighbors += neighbor->*value;
    }
    return sum_neighbors -
           static_cast<double>(neighbors.size()) * (current_cell.*value);
}

void UpdateCells(std::vector<Cell>& current_grid,
                 std::vector<Cell>& next_grid) {
    ForEachCell([&](int row, int column) {
        const int index = row * columns + column;
        const Cell& current_cell = current_grid[index];
        Cell& next_cell = next_grid[index];

        const double laplacian_a =
            CalculateLaplacian(current_grid, index, &Cell::a);
        const double laplacian_b =
            CalculateLaplacian(current_grid, index, &Cell::b);

        const double reaction =
            current_cell.a * current_cell.b * current_cell.b;

        next_cell.a = current_cell.a + diffusion_a * laplacian_a - reaction +
                      feed_rate * (1.0 - current_cell.a);
        next_cell.b = current_cell.b + diffusion_b * laplacian_b + reaction -
                      (kill_rate + feed_rate) * current_cell.b;
    });

    std::swap(current_grid, next_grid);
}

int main() {
    std::vector<Cell> current_grid(rows * columns);
    std::vector<Cell> next_grid(rows * columns);

    bool viewing_b = true;
    bool reset_this_frame = true;

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}),
                            "Diffusion Garden");

    window.setFramerateLimit(30);

    sf::RectangleShape cell({cell_width, cell_height});

    ResetSimulation(current_grid, next_grid);  // Seed

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* key_pressed =
                           event->getIf<sf::Event::KeyPressed>()) {
                if (key_pressed->scancode == sf::Keyboard::Scancode::R) {
                    ResetSimulation(current_grid, next_grid);
                    reset_this_frame = true;
                } else if (key_pressed->scancode ==
                           sf::Keyboard::Scancode::Num1) {
                    viewing_b = true;
                } else if (key_pressed->scancode ==
                           sf::Keyboard::Scancode::Num2) {
                    viewing_b = false;
                }
            }
        }
        if (!reset_this_frame) {
            constexpr int updates_per_frame =
                10;  // approx 300 timesteps per second

            for (int i = 0; i < updates_per_frame; ++i) {
                UpdateCells(current_grid, next_grid);
            }
        }
        reset_this_frame = false;

        window.clear();

        ForEachCell([&](int row, int column) {
            const int index = row * columns + column;
            const Cell& current_cell = current_grid[index];

            sf::Vector2f position = {column * cell_width, row * cell_height};
            cell.setPosition(position);

            if (!viewing_b) {
                const double concentration =
                    std::clamp(current_cell.a, 0.0, 1.0);
                const std::uint8_t intensity =
                    static_cast<std::uint8_t>(concentration * 255.0);
                // blue
                cell.setFillColor(
                    sf::Color(255 - intensity, 255 - intensity, 255));
            } else {
                const double concentration =
                    std::clamp(current_cell.b, 0.0, 1.0);
                const std::uint8_t color_intensity =
                    static_cast<std::uint8_t>(concentration * 255.0);
                // green
                cell.setFillColor(sf::Color(255 - color_intensity, 255,
                                            255 - color_intensity));
            }
            window.draw(cell);
        });
        window.display();
    }
}
