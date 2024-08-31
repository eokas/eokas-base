
#include "./header.h"
#include "./schema.h"
#include "./library.h"
using namespace eokas::datapot;

int main(int argc, char** argv)
{
    Library lib("libHero");
    
    StructSchema heroSchema("Hero");
    lib.addSchema(&heroSchema);
    heroSchema.addMember("ID", lib.getSchama("Int"));
    heroSchema.addMember("Name", lib.getSchama("String"));
    heroSchema.addMember("Level", lib.getSchama("Int"));
    heroSchema.addMember("Power", lib.getSchama("Float"));
    
    ListSchema heroListSchema("HeroList");
    
    Value* heroList = lib.make("HeroList");
    
    Value* hero = lib.make("Hero");
    Value* heroID = lib.get(hero, "ID");
    Value* heroName = lib.get(hero, "Name");
    Value* heroLevel = lib.get(hero, "Level");
    
    lib.set(heroID, 1001);
    lib.set(heroName, "Dawn");
}
