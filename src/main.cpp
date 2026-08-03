#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

// * Small, fast-growing space
// constexpr int window_width = 600;
// constexpr int window_height = 600;
// constexpr int rows = 300;
// constexpr int columns = 300;

// * Medium, slow-growing space
constexpr int window_width = 800;
constexpr int window_height = 800;
constexpr int rows = 600;
constexpr int columns = 600;

// * Large, very-slow-growing space
// constexpr int window_width = 800;
// constexpr int window_height = 800;
// constexpr int rows = 1201;
// constexpr int columns = 1201;

struct Cell {
    double a = 1.0;
    double b = 0.0;
};

constexpr double diffusion_a = 0.2;
constexpr double diffusion_b = 0.1;
constexpr double feed_rate = 0.05;
constexpr double kill_rate = 0.06;

constexpr double seed_a = 0.0;
constexpr double seed_b = 1.0;

const int center_row = rows / 2;
const int center_column = columns / 2;
const int center_index = center_row * columns + center_column;

// Defines seeded chemical values.
void ResetSimulation(std::vector<Cell>& current_grid,
                     std::vector<Cell>& next_grid) {
    current_grid.assign(rows * columns, Cell{});
    next_grid.assign(rows * columns, Cell{});

    // * One central seed
    for (int row = center_row - 3; row <= center_row + 3; ++row) {
        for (int column = center_column - 3; column <= center_column + 3;
             ++column) {
            const int index = row * columns + column;
            current_grid[index].a = seed_a;
            current_grid[index].b = seed_b;
        }
    }

    // * Four central seeds
    // constexpr int seed_radius = 3;
    // const int offset = 50;

    // const std::pair<int, int> seed_centers[] = {
    //     {center_row - offset, center_column - offset},
    //     {center_row - offset, center_column + offset},
    //     {center_row + offset, center_column - offset},
    //     {center_row + offset, center_column + offset},
    // };

    // for (const auto& [seed_row, seed_column] : seed_centers) {
    //     for (int row = seed_row - seed_radius; row <= seed_row + seed_radius;
    //          ++row) {
    //         for (int column = seed_column - seed_radius;
    //              column <= seed_column + seed_radius; ++column) {
    //             const int index = row * columns + column;

    //             current_grid[index].a = 0.0;
    //             current_grid[index].b = 1.0;
    //         }
    //     }
    // }
}

template <typename Callable>
void ForEachCell(Callable callable) {
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            callable(row, column);
        }
    }
}

// Calculates the concentration difference with neighboring cells.
// Positive: neighbors have more. Negative: current cell has more.
double CalculateLaplacian(const std::vector<Cell>& current_grid, int row,
                          int column, int index, double Cell::* value) {
    const Cell& current_cell = current_grid[index];

    double sum_neighbors = 0.0;
    int neighbor_count = 0;

    if (row != 0) {
        sum_neighbors += current_grid[index - columns].*value;
        ++neighbor_count;
    }
    if (column != columns - 1) {
        sum_neighbors += current_grid[index + 1].*value;
        ++neighbor_count;
    }
    if (row != rows - 1) {
        sum_neighbors += current_grid[index + columns].*value;
        ++neighbor_count;
    }
    if (column != 0) {
        sum_neighbors += current_grid[index - 1].*value;
        ++neighbor_count;
    }

    return sum_neighbors -
           static_cast<double>(neighbor_count) * (current_cell.*value);
}

void UpdateCells(std::vector<Cell>& current_grid,
                 std::vector<Cell>& next_grid) {
    ForEachCell([&](int row, int column) {
        const int index = row * columns + column;
        const Cell& current_cell = current_grid[index];
        Cell& next_cell = next_grid[index];

        const double laplacian_a =
            CalculateLaplacian(current_grid, row, column, index, &Cell::a);
        const double laplacian_b =
            CalculateLaplacian(current_grid, row, column, index, &Cell::b);

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

    bool reset_this_frame = true;
    bool paused = false;

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}),
                            "Diffusion Garden");

    std::vector<std::uint8_t> pixels(rows * columns * 4);  // 4 values: RGBA

    sf::Texture texture(sf::Vector2u{static_cast<unsigned int>(columns),
                                     static_cast<unsigned int>(rows)});
    texture.setSmooth(false);

    sf::Sprite sprite(texture);
    sprite.setScale({static_cast<float>(window_width) / columns,
                     static_cast<float>(window_height) / rows});

    window.setFramerateLimit(30);

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
                           sf::Keyboard::Scancode::Space) {
                    paused = !paused;
                }
            }
        }
        if (!paused && !reset_this_frame) {
            constexpr int updates_per_frame =
                10;  // Up to 300 timesteps per second at 30 FPS.

            for (int i = 0; i < updates_per_frame; ++i) {
                UpdateCells(current_grid, next_grid);
            }
        }
        reset_this_frame = false;

        window.clear();

        ForEachCell([&](int row, int column) {
            const int index = row * columns + column;
            const Cell& current_cell = current_grid[index];
            const int pixel_offset = index * 4;

            const double concentration = std::clamp(current_cell.b, 0.0, 1.0);
            const std::uint8_t intensity =
                static_cast<std::uint8_t>(concentration * 255.0);

            constexpr std::uint8_t red = 66;
            constexpr std::uint8_t green = 110;
            constexpr std::uint8_t blue = 52;

            pixels[pixel_offset] = 255 - ((255 - red) * intensity) / 255;
            pixels[pixel_offset + 1] = 255 - ((255 - green) * intensity) / 255;
            pixels[pixel_offset + 2] = 255 - ((255 - blue) * intensity) / 255;
            pixels[pixel_offset + 3] = 255;
        });
        texture.update(pixels.data());
        window.draw(sprite);
        window.display();
    }
}
