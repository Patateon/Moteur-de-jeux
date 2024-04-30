
// Others
#include <common/transform.hpp>
#include <common/entity.hpp>

Entity::Entity(Mesh * mesh){
    this->m_currentMesh = mesh;
}

void Entity::move(){
    this->m_transform.translation = this->m_movement.position;
    this->updateSelfAndChild();
}


void Entity::addChild(Entity * child){
    m_children.emplace_back(child);
    m_children.back()->m_parent = this;
}


void Entity::updateSelfAndChild()
{
    if (m_parent)
        m_transform.model = m_parent->m_transform.model * 
            m_transform.getLocalModel();
    else
        m_transform.model = m_transform.getLocalModel();

    for (auto&& child : m_children)
    {
        child->updateSelfAndChild();
    }
}

void Entity::init(){
    this->getMesh()->init();
}

void Entity::render(){
    this->getMesh()->render();
}

void Entity::clear(){
    m_children.clear();
    this->getMesh()->clear();
}
