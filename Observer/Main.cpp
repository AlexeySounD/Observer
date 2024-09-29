#include <iostream>
#include <string>
#include <vector>

using namespace std;

class IObserver
{
public:
    virtual void update(int health) = 0;
};

class IObservable
{
public:
    virtual void addObserver(IObserver* o) = 0;
    virtual void removeObserver(IObserver* o) = 0;
    virtual void notify() = 0;
    virtual ~IObservable() = default;
};

class Character : public IObservable
{
private:
    vector<IObserver*> observers;
    int health;

public:
    Character(int h) : health(h) {}
    
    void takeDamage(int damage)
    {
        health -= damage;
        if (health < 0) health = 0;
        notify();  
    }

    int getHealth() const { return health; }
   
    void addObserver(IObserver* o) override
    {
        observers.push_back(o);
    }
    
    void removeObserver(IObserver* o) override
    {
        observers.erase(remove(observers.begin(), observers.end(), o), observers.end());
    }
   
    void notify() override
    {
        for (auto o : observers)
        {
            o->update(health);
        }
    }
};

class GameUI : public IObserver
{
public:
    void update(int health) override
    {
        cout << "UI: Character's health changed on : " << health << endl;
    }
};

class Logger : public IObserver
{
public:
    void update(int health) override
    {
        cout << "Log: new state of characters's health: " << health << endl;
    }
};

class EnemyAI : public IObserver
{
public:
    void update(int health) override
    {
        if (health < 50)
        {
            cout << "Enemy: character weakened! Let's attack him!" << endl;
        }
    }
};

int main()
{
    
    Character* character = new Character(100);
        
    GameUI* ui = new GameUI();
    Logger* logger = new Logger();
    EnemyAI* enemy = new EnemyAI();

    character->addObserver(ui);
    character->addObserver(logger);
    character->addObserver(enemy);

    cout << "Character get 30 damage..." << endl;
    character->takeDamage(30);

    cout << "\nCharacter get 50 damage..." << endl;
    character->takeDamage(50);
       
    character->removeObserver(enemy);

    cout << "\nCharacter get 40 damage..." << endl;
    character->takeDamage(40);

    delete character;
    delete ui;
    delete logger;
    delete enemy;

    return 0;
}