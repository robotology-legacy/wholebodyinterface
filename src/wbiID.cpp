/*
 * Copyright (C) 2013  CoDyCo Consortium
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 *
 * Authors: Silvio Traversaro, Andrea Del Prete, Marco Randazzo
 * email: silvio.traversaro@iit.it - andrea.delprete@iit.it - marco.randazzo@iit.it
 */

#include "wbiID.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace wbi {

ID::ID()
{
    id = "WBI_ID_DEFAULT_VALUE";
}

ID::ID(const std::string & _id)
{
    id = _id;
}

ID::ID(const char * _id)
{
    id = std::string(_id);
}


const std::string & ID::toString() const
{
    return id;
}

ID & ID::operator=(const ID & id_copy)
{
    this->id = id_copy.toString();
    return *this;
}

bool ID::operator==(const ID & comparison_id) const
{
    return (comparison_id.toString() == this->id);
}


IDList::IDList()
{}
IDList::IDList(const ID &id0)
{ addID(id0); }
IDList::IDList(const ID &id0, const ID &id1)
{ addID(id0); addID(id1);}
IDList::IDList(const ID &id0, const ID &id1, const ID &id2)
{ addID(id0); addID(id1); addID(id2); }
IDList::IDList(const ID &id0, const ID &id1, const ID &id2, const ID &id3)
{ addID(id0); addID(id1); addID(id2); addID(id3); }
IDList::IDList(const ID &id0, const ID &id1, const ID &id2, const ID &id3, const ID &id4)
{ addID(id0); addID(id1); addID(id2); addID(id3); addID(id4); }
IDList::IDList(const ID &id0, const ID &id1, const ID &id2, const ID &id3, const ID &id4, const ID& id5)
{ addID(id0); addID(id1); addID(id2); addID(id3); addID(id4); addID(id5); }
IDList::IDList(const ID &id0, const ID &id1, const ID &id2, const ID &id3, const ID &id4, const ID& id5, const ID & id6)
{ addID(id0); addID(id1); addID(id2); addID(id3); addID(id4); addID(id5); addID(id6); }

IDList::IDList(const IDList &lid1, const IDList &lid2)
{ addIDList(lid1); addIDList(lid2); }
IDList::IDList(const IDList &lid1, const IDList &lid2, const IDList &lid3)
{ addIDList(lid1); addIDList(lid2); addIDList(lid3); }
IDList::IDList(const IDList &lid1, const IDList &lid2, const IDList &lid3, const IDList &lid4)
{ addIDList(lid1); addIDList(lid2); addIDList(lid3); addIDList(lid4); }
IDList::IDList(const IDList &lid1, const IDList &lid2, const IDList &lid3, const IDList &lid4, const IDList &lid5)
{ addIDList(lid1); addIDList(lid2); addIDList(lid3); addIDList(lid4); addIDList(lid5); }
IDList::IDList(const IDList &lid1, const IDList &lid2, const IDList &lid3, const IDList &lid4, const IDList &lid5, const IDList &lid6)
{ addIDList(lid1); addIDList(lid2); addIDList(lid3); addIDList(lid4); addIDList(lid5); addIDList(lid6); }

IDList::~IDList() {}

void IDList::pushID(const ID &id)
{
    storage.push_back(id);
}

bool IDList::idToIndex(const ID &_wbiId, int & numericId) const
{
    ID wbi_id = _wbiId;
    std::vector<ID>::const_iterator it = std::find(storage.begin(), storage.end(), wbi_id);
    if (it == storage.end())
    {
        return false;
    }
    numericId = std::distance(storage.begin(), it);
    return true;
}

/** Convert a global id into a local id */
bool IDList::indexToID(const int numeridId, ID & wbi_id) const
{
    if (numeridId < 0 && numeridId >= this->size())
    {
        return false;
    }
    wbi_id = storage[numeridId];
    return true;
}

//Remove an existing element
bool IDList::removeID(const ID &j)
{
    std::vector<ID>::iterator it = std::find(storage.begin(), storage.end(), j);
    if( it == storage.end() )
    {
        return false;
    }
    storage.erase(it);
    return true;
}

bool IDList::addID(const ID &i)
{
    if (containsID(i))
        return true;
    pushID(i);
    return true;
}

int IDList::addIDList(const IDList &addedList)
{
    int count = 0;
    ID added_id;
    for (int i = 0; i < addedList.size(); i++)
    {
        addedList.indexToID(i,added_id);
        if(!containsID(added_id))
        {
            pushID(added_id);
            count++;
        }
    }
    return count;
}

bool IDList::containsID(const ID &i) const
{
    std::vector<ID>::const_iterator it = std::find(storage.begin(), storage.end(), i);
    if (it == storage.end()) {
        return false;
    } else {
        return true;
    }
}

// Get the number of ids in this list
unsigned int IDList::size() const
{
    return storage.size();
}

std::string IDList::toString() const
{
    std::ostringstream s;
    for (int i = 0; i < storage.size(); i++)
    {
        s << storage[i].toString() << std::endl;
    }
    return s.str();
}

const wbi::ID& IDList::at(unsigned index) const
{
    if (index >= size()) throw std::out_of_range("IDList out of range");
    return storage[index];
}

void IDList::removeAllIDs()
{
    this->storage.clear();
}

}
