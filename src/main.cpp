#include <iostream>
#include <string>

// 地図の型定義
using Map = std::vector<std::vector<char>>;

// 車の向きを表す列挙型
enum class Direction {
    North,
    East,
    South,
    West
};

// 車の状態を表す構造体
struct CarState {
    int x;           // 車のX座標
    int y;           // 車のY座標
    Direction dir;   // 車の向き
    int gas;        // ガスの容量
    int vel;        // 車の速度
};

// 地図を標準出力する関数
void printMap(const Map& map) {
    for (const auto& row : map) {
        for (char cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Directionを文字列に変換する関数
std::string directionToString(Direction dir) {
    switch (dir) {
        case Direction::North:
            return "North";
        case Direction::East:
            return "East";
        case Direction::South:
            return "South";
        case Direction::West:
            return "West";
        default:
            return "Unknown";
    }
}

// 車を表示する関数
void displayCar(const CarState& car, Map& map) {
    // 地図上に車を表示
    map[car.x][car.y] = 'C';

    // 地図を標準出力
    for (const auto& row : map) {
        for (char cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    // 車の向きと位置を表示
    std::cout << "Car Position: (" << car.x << ", " << car.y << "), Direction: ";
    switch (car.dir) {
        case Direction::North:
            std::cout << "North";
            break;
        case Direction::East:
            std::cout << "East";
            break;
        case Direction::South:
            std::cout << "South";
            break;
        case Direction::West:
            std::cout << "West";
            break;
    }
    std::cout << std::endl;
}

// 車を動かす関数
bool moveCar(CarState& car, const std::string& command, const Map &map) {
    const int mapSize = 10;
    
    if (command == "turn left") {
        // 左に回転
        switch (car.dir) {
            case Direction::North:
                car.dir = Direction::West;
                break;
            case Direction::East:
                car.dir = Direction::North;
                break;
            case Direction::South:
                car.dir = Direction::East;
                break;
            case Direction::West:
                car.dir = Direction::South;
                break;
        }
    } else if (command == "turn right") {
        // 右に回転
        switch (car.dir) {
            case Direction::North:
                car.dir = Direction::East;
                break;
            case Direction::East:
                car.dir = Direction::South;
                break;
            case Direction::South:
                car.dir = Direction::West;
                break;
            case Direction::West:
                car.dir = Direction::North;
                break;
        }
    } else if (command == "continue straight") {
        // 直進
        switch (car.dir) {
            case Direction::North: {
                int tmp_car_y = car.y + car.vel;
                if (tmp_car_y < map.size()) {
                    car.y = tmp_car_y;
                    car.gas -= car.vel;
                } else {
                    std::cout << "cannot access that cell" << std::endl;
                }
                break;
            }
            case Direction::East: {
                int tmp_car_x = car.x - car.vel;
                if (tmp_car_x >= 0) {
                    car.x = tmp_car_x;
                    car.gas -= car.vel;
                } else {
                    std::cout << "cannot access that cell" << std::endl;
                }
                break;
            }
            case Direction::South: {
                int tmp_car_y = car.y - car.vel;
                if (tmp_car_y >= 0) {
                    car.y = tmp_car_y;
                    car.gas -= car.vel;
                } else {
                    std::cout << "cannot access that cell" << std::endl;
                }
                break;
            }
            case Direction::West: {
                int tmp_car_x = car.x + car.vel;
                if (tmp_car_x < map[0].size()) {
                    car.x = tmp_car_x;
                    car.gas -= car.vel;
                } else {
                    std::cout << "cannot access that cell" << std::endl;
                }
                break;
            }
        }
    } else if (command == "accelerate") {
        // 加速
        if(car.vel < 5){
        car.vel++;
        }
        else{
            std::cout << "car velocity's max velocity is 5" << std::endl;
        }
    } else if (command == "decelerate") {
        // 減速
        if(car.vel > 0){
        car.vel--;
        }
        else{
            std::cout << "car velocity's min velocity is 1" << std::endl;
        }        
    } else if (command == "stop") {
        // 停止
        car.vel = 0;
    } else {
        std::cout << "Invalid command for the car." << std::endl;
        return false;
    }
    return true;
}

// ガスステーションの位置をチェックする関数
bool checkGasStation(const CarState& car, const Map& map) {
    char landmarkSymbols[] = {'S'};

    for (char symbol : landmarkSymbols) {
        for (int i = std::max(0, car.y - 1); i <= std::min(static_cast<int>(map.size()) - 1, car.y + 1); ++i) {
            for (int j = std::max(0, car.x - 1); j <= std::min(static_cast<int>(map[0].size()) - 1, car.x + 1); ++j) {
                if (map[i][j] == symbol) {
                    if(i == car.y && j == car.x){
                        return true;
                    }
                    else{
                    std::cout << "Gas Station is nearby!" << std::endl;
                        return false;
                    }
                }
            }
        }
    }
    return false;
}

// ゴールの位置をチェックする関数
bool checkGoal(const CarState& car, const Map& map) {
    char landmarkSymbols[] = {'G'};

    for (char symbol : landmarkSymbols) {
        for (int i = std::max(0, car.y - 1); i <= std::min(static_cast<int>(map.size()) - 1, car.y + 1); ++i) {
            for (int j = std::max(0, car.x - 1); j <= std::min(static_cast<int>(map[0].size()) - 1, car.x + 1); ++j) {
                if (map[i][j] == symbol) {
                    if(i == car.y && j == car.x){
                        return true;
                    }
                    else{
                    std::cout << "Goal is nearby!" << std::endl;
                        return false;
                    }
                }
            }
        }
    }
    return false;
}


int main() {
    // 地図の初期化 (10x10)
    Map map = {
        {'-', '-', '-', '-', '-','-', '-', '-', '-', '-'},
        {'-', '-', 'S', '-', '-','-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-','-', '-', '-', '-', '-'},
        {'-', 'S', '-', '-', '-','-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-','-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-','-', 'S', '-', '-', '-'},
        {'-', '-', '-', '-', '-','-', '-', '-', 'S', '-'},
        {'-', '-', '-', '-', '-','-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-','S', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-','-', '-', '-', '-', 'G'},
    };

    // ガスの初期値
    const int life = 10;

    // 初期状態の車を作成
    CarState myCar = {0, 0, Direction::North, life, 1};
    // 更新時のmap
    Map update_map = map;

    // 初期状態の地図を標準出力
    update_map[myCar.y][myCar.x] = 'C';
    std::cout << "<Initial Map>" << std::endl;
        std::cout << "Car pos : " << myCar.x << ", " << myCar.y << std::endl;
        std::cout << "Gas status : " << myCar.gas << std::endl;
        std::cout << "Car direction : " << directionToString(myCar.dir) << std::endl;
        std::cout << "Car velocity : " << myCar.vel << std::endl;
    printMap(update_map);

    std::string command;

    // ステップごとに地図を更新して標準出力
    while(1){
    // mapの初期化
    Map update_map = map;

    // 標準入力からコマンドを受け取り、処理する
    while (true) {
        std::cout << "Enter a command: ";
        std::getline(std::cin, command);

        if(moveCar(myCar, command, map)){
            break;
        }
    }
        update_map[myCar.y][myCar.x] = 'C';
        if(checkGasStation(myCar, map)){
            myCar.gas = life;
        };
        std::cout << "========================" << std::endl;
        std::cout << "Car pos : " << myCar.x << ", " << myCar.y << std::endl;
        std::cout << "Gas status : " << myCar.gas << std::endl;
        std::cout << "Car direction : " << directionToString(myCar.dir) << std::endl;
        std::cout << "Car velocity : " << myCar.vel << std::endl;
        printMap(update_map);
        if(myCar.gas == 0 ){
            std::cout << "your car is empty !" << std::endl;
            break;
        }
        if(checkGoal(myCar, map)){
            std::cout << "Goal !!!! " << std::endl;
            break;
        };
        std::cout << "========================" << std::endl;
    }

    return 0;
}