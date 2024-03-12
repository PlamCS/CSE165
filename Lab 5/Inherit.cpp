#include "Animal.h"
#include "Dog.h"
#include "display.h"
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
    vector<Animal*> animals;
    int n;
    cin>>n;
    char type;
    string name;
    int age;
    for(int i = 0; i < n; i++){
        cin>>type;
        cin>>name;
        cin>>age;
        Animal* animal;
        if(type == 'D'){
            animal = new Dog();

        }else{
            animal = new Animal();
        }
        animal->setName(name);
        animal->setAge(age);
        animals.push_back(animal);
    }

    display(animals);

    return 0;
}
