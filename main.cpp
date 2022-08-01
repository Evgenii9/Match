#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

int ts = 54;
Vector2i offset(48, 24);

struct Piece
{
    int x, y, col, row, kind, match, alpha;
    Piece() {
        match = 0;
        alpha = 255;
    }
} grid[10][10];

void swap(Piece p1, Piece p2) {
    swap(p1.col, p2.col);
    swap(p1.row, p2.row);

    grid[p1.row][p1.col] = p1;
    grid[p2.row][p2.col] = p2;
}

class setSound {
public:
    SoundBuffer s1, s2, s3;
    Sound pusk, score, error;

    setSound() {
        s1.loadFromFile("C:/Users/ой/Desktop/Programming_Project/Lesson/Play/Play_Match/Match/pusk.ogg");
        s2.loadFromFile("C:/Users/ой/Desktop/Programming_Project/Lesson/Play/Play_Match/Match/score.ogg");
        s3.loadFromFile("C:/Users/ой/Desktop/Programming_Project/Lesson/Play/Play_Match/Match/error.ogg");

        pusk.setBuffer(s1);
        score.setBuffer(s2);
        error.setBuffer(s3);
    }
};

setSound sound;

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(520, 480), "Match!");
    window.setFramerateLimit(60);

    Texture f;
    f.loadFromFile("C:/Users/ой/Desktop/Programming_Project/Lesson/Play/Play_Match/Match/Painting/fon.png");
    Sprite fon(f);

    Texture b;
    b.loadFromFile("C:/Users/ой/Desktop/Programming_Project/Lesson/Play/Play_Match/Match/Painting/blocks.png");
    Sprite blocks(b);

    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++) {
            grid[i][j].kind = rand() % 7;

            grid[i][j].col = j;
            grid[i][j].row = i;

            grid[i][j].x = j * ts;
            grid[i][j].y = i * ts;
        }

    int x0, y0, x, y;
    int click = 0;
    Vector2i pos;
    bool isSwap = false, isMoving = false;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
                if (event.key.code == Mouse::Left) {
                    sound.pusk.play();

                    if (!isSwap && !isMoving)
                        click++;

                    pos = Mouse::getPosition(window) - offset;
                }
        }

        if (click == 1) {
            x0 = pos.x / ts + 1;
            y0 = pos.y / ts + 1;
        }
        if (click == 2) {
            x = pos.x / ts + 1;
            y = pos.y / ts + 1;

            if (abs(x - x0) + abs(y - y0) == 1) {
                swap(grid[y0][x0], grid[y][x]);
                isSwap = true;
                click = 0;
            }
            else click = 1;
        }

        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++) {
                if (grid[i][j].kind == grid[i + 1][j].kind)
                    if (grid[i][j].kind == grid[i - 1][j].kind)
                        for (int n = -1; n <= 1; n++)
                            grid[i + n][j].match++;

                if (grid[i][j].kind == grid[i][j + 1].kind)
                    if (grid[i][j].kind == grid[i][j - 1].kind)
                        for (int n = -1; n <= 1; n++)
                            grid[i][j + n].match++;
            }

        isMoving = false;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++) {
                Piece& p = grid[i][j];
                int dx, dy;

                for (int n = 0; n < 4; n++) {
                    dx = p.x - p.col * ts;
                    dy = p.y - p.row * ts;

                    if (dx)
                        p.x -= dx / abs(dx);
                    if (dy)
                        p.y -= dy / abs(dy);
                }

                if (dx || dy)
                    isMoving = true;
            }

        if (!isMoving)
            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++)
                    if (grid[i][j].match)
                        if (grid[i][j].alpha > 10) {
                            grid[i][j].alpha -= 10;
                            isMoving = true;
                        }

        int score = 0;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
                score += grid[i][j].match;

        if (isSwap && !isMoving) {
            if (!score) {
                swap(grid[y0][x0], grid[y][x]);

                sound.error.play();
            }
            else
                sound.score.play();

            isSwap = false;
        }

        if (!isMoving) {
            for (int i = 8; i > 0; i--)
                for (int j = 1; j <= 8; j++)
                    if (grid[i][j].match)
                        for (int n = i; n > 0; n--)
                            if (!grid[n][j].match) {
                                swap(grid[n][j], grid[i][j]);
                                break;
                            }

            for (int j = 1; j <= 8; j++)
                for (int i = 8, n = 0; i > 0; i--)
                    if (grid[i][j].match) {
                        grid[i][j].kind = rand() % 7;

                        grid[i][j].y = -ts * n++;
                        grid[i][j].match = 0;

                        grid[i][j].alpha = 255;
                    }
        }

        window.clear(Color::White);
        window.draw(fon);

        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++) {
                Piece p = grid[i][j];

                blocks.setTextureRect(IntRect(p.kind * 49, 0, 49, 49));
                blocks.setColor(Color(255, 255, 255, p.alpha));
                blocks.setPosition(p.x, p.y);
                blocks.move(offset.x - ts, offset.y - ts);

                window.draw(blocks);
            }

        window.display();
    }
    return 0;
}