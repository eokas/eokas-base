#include "./schema.h"

namespace eokas::datapot {

    Schema::Schema(SchemaType type, const String& name)
        : mType(type)
        , mName(name) {
        if(mType == SchemaType::List) {
            mBody.listBody = new ListBody();
        }
        else if(mType == SchemaType::Struct) {
            mBody.structBody = new StructBody();
        }
    }

    Schema::~Schema() {
        if(mType == SchemaType::List) {
            _DeletePointer(mBody.listBody);
        }
        if(mType == SchemaType::Struct) {
            _DeletePointer(mBody.structBody);
        }
    }

    bool Schema::operator==(const Schema& other) const {
        return mType == other.mType && mName == other.mName;
    }

    bool Schema::operator!=(const Schema& other) const {
        return mType != other.mType || mName != other.mName;
    }

    void Schema::setElement(Schema* schema) {
        if(schema == this)
            return;
        if(mType == SchemaType::List) {
            mBody.listBody->element = schema;
        }
    }

    Schema* Schema::getElement() const {
        if(mType == SchemaType::List) {
            return mBody.listBody->element;
        }
        return nullptr;
    }

    void Schema::addMember(const String& name, Schema* schema) {
        if(mType != SchemaType::Struct)
            return;

        Member& mem = mBody.structBody->members.emplace_back();
        mem.name = name;
        mem.schema = schema;
    }

    void Schema::setMember(const String& name, Schema* schema) {
        if(mType != SchemaType::Struct)
            return;

        Member* exists = this->getMember(name);
        if(exists != nullptr) {
            exists->schema = schema;
        }
        else {
            this->addMember(name, schema);
        }
    }

    void Schema::delMember(const String& name) {
        if(mType != SchemaType::Struct)
            return;

        auto iter = mBody.structBody->members.begin();
        while(iter != mBody.structBody->members.end()) {
            if(iter->name == name) {
                iter = mBody.structBody->members.erase(iter);
                continue;
            }
            ++ iter;
        }
    }

    Schema::Member* Schema::getMember(const String& name) {
        if(mType != SchemaType::Struct)
            return nullptr;

        for(auto& mem : mBody.structBody->members) {
            if(mem.name == name)
                return &mem;
        }
        return nullptr;
    }

    Schema::Member* Schema::getMember(size_t index) {
        if(mType != SchemaType::Struct)
            return nullptr;
        return &mBody.structBody->members.at(index);
    }

    size_t Schema::getMemberCount() {
        if(mType != SchemaType::Struct)
            return 0;
        return mBody.structBody->members.size();
    }

    size_t Schema::getMemberIndex(const String& name) {
        if(mType != SchemaType::Struct)
            return -1;
        for(size_t i = 0; i < mBody.structBody->members.size(); i++) {
            if(mBody.structBody->members[i].name == name) {
                return i;
            }
        }
        return -1;
    }

    SchemaHeap::SchemaHeap()
        : mSchemas() {

        this->add(SchemaType::Int, "Int");
        this->add(SchemaType::Float, "Float");
        this->add(SchemaType::Bool, "Bool");
        this->add(SchemaType::String, "String");
    }

    SchemaHeap::~SchemaHeap() {
        _DeleteMap(mSchemas);
    }

    Schema* SchemaHeap::add(SchemaType type, const String& name) {
        auto iter = mSchemas.find(name);
        if(iter != mSchemas.end())
            return nullptr;

        auto* schema = new Schema(type, name);
        mSchemas.insert(std::make_pair(name, schema));

        return schema;
    }

    Schema* SchemaHeap::get(const String& name) {
        auto iter = mSchemas.find(name);
        if(iter == mSchemas.end())
            return nullptr;
        return iter->second;
    }
}
