
#include "datapot/header.h"
#include "datapot/schema.h"
#include "datapot/library.h"
using namespace eokas::datapot;

int main(int argc, char** argv)
{
    Library lib("libHero");

    Schema* schemaHero = lib.addSchema(SchemaType::Struct, "Hero");
    schemaHero->addMember("ID", lib.getSchama("Int"));
    schemaHero->addMember("Name", lib.getSchama("String"));
    schemaHero->addMember("Level", lib.getSchama("Int"));
    schemaHero->addMember("Power", lib.getSchama("Float"));

    Schema* schemaHeroList = lib.addSchema(SchemaType::List, "HeroList");
    schemaHeroList->setElement(schemaHero);

    Value* heroList = lib.make("HeroList");
    
    Value* hero = lib.make("Hero");
    lib.set(hero, "ID", 100);
    lib.set(hero, "Name", "Dawn");
    lib.set(hero, "Level", 10);

    lib.set(heroList, 0, hero);
}
