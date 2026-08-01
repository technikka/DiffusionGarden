#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

constexpr int window_width = 600;
constexpr int window_height = 600;

constexpr int rows = 40;
constexpr int columns = 40;

constexpr float cell_width = static_cast<float>(window_width) / columns;
constexpr float cell_height = static_cast<float>(window_height) / rows;

struct Cell {
    double a = 1.0;
    double b = 0.0;
};

void UpdateCells(std::vector<Cell>& current_grid,
                 std::vector<Cell>& next_grid) {
    double concentration = 0.0;
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            const int index = row * columns + column;
            const Cell& current_cell = current_grid[index];
            Cell& next_cell = next_grid[index];

            next_cell.b = concentration;
            if (row == column && row % 3 == 0) {
                concentration += 0.05;
            }
        }
    }
    std::swap(current_grid, next_grid);
}

int main() {
    std::vector<Cell> current_grid(rows * columns);
    std::vector<Cell> next_grid(rows * columns);

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}),
                            "Diffusion Garden");

    sf::RectangleShape cell({cell_width, cell_height});

    // Initialization
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
        }
    }

    // Rendering
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        UpdateCells(current_grid, next_grid);
        window.clear();

        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                const int index = row * columns + column;
                Cell& current_cell = current_grid[index];

                sf::Vector2f position = {column * cell_width,
                                         row * cell_height};

                cell.setPosition(position);

                const double concentration =
                    std::clamp(current_cell.b, 0.0, 1.0);

                const std::uint8_t intensity =
                    static_cast<std::uint8_t>(concentration * 255.0);

                cell.setFillColor(
                    sf::Color(255 - intensity, 255, 255 - intensity));

                window.draw(cell);
            }
        }

        window.display();
    }
}
