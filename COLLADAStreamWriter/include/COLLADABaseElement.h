/*
    Copyright (c) 2008 NetAllied Systems GmbH
	
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#ifndef __COLLADASTREAMWRITER_BASE_ELEMENT_H__
#define __COLLADASTREAMWRITER_BASE_ELEMENT_H__

#include "COLLADAPrerequisites.h"
#include "COLLADAElementWriter.h"

namespace COLLADA
{

    /** Template class representing a base element. */
//    template<const String& mElementName>
    class BaseElement : public ElementWriter
    {
    protected:

        /** Name of the element. */
        String mElementName;

        /** The id of the vertex element*/
        String mNodeId;

        /** The name of the vertex node*/
        String mNodeName;

    public:

        /**
         * Constructor.
         * @param streamWriter Pointer to the stream.
         */
        BaseElement ( StreamWriter* streamWriter, const String& elementName )
        : ElementWriter ( streamWriter )
        , mElementName( elementName )
        , mNodeId ( "" )
        , mNodeName ( "" )
        {}

        /**
         * Destructor.
         */
        virtual ~BaseElement() {};

        /** Returns a reference to the id of the element*/
        const String& getId() const
        {
            return mNodeId;
        }

        /** Sets the id of the element*/
        void setId ( const String& id )
        {
            mNodeId = id;
        }

        /** Returns a reference to the name of the element*/
        const String& getNodeName() const
        {
            return mNodeName;
        }

        /** Sets the id of the element*/
        void setNodeName ( const String& nodeName )
        {
            mNodeName = nodeName;
        }

        /** Adds the element to the stream */
        void add();

    };

    //---------------------------------------------------------------

    /**
    * Template class to define the typedefs.
    */
    template<const String& elementName>
    class BaseElementTemplate : public BaseElement
    {
    public:
        BaseElementTemplate ( StreamWriter* streamWriter ) 
            : BaseElement( streamWriter, elementName ) {};
    };


} //namespace COLLADA


#endif //__COLLADASTREAMWRITER_BASE_ELEMENT_H__