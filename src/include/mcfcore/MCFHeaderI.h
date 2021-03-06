/*
Copyright (C) 2011 Mark Chandler (Desura Net Pty Ltd)
Copyright (C) 2014 Bad Juju Games, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.

Contact us at legal@badjuju.com.
*/

#ifndef DESURA_MCF_HEADER_I_H
#define DESURA_MCF_HEADER_I_H

#include "DesuraId.h"

#define MCF_HEADER_FACTORY "MCF_HEADER"

#define MCF_HEADERSIZE_V1 31
#define MCF_HEADERSIZE_V2 35

//#define MCF_HEADERSIZE 35
#define MCF_CURRENTVERSION 0x02

namespace MCFCore
{
	//! This is an Interface for a Mcf Header
	//! @see MCFHeader
	class MCFHeaderI
	{
	public:
		enum MCFHeaderFlags
		{
			NO_FLAGS			= 0<<0,		//!< No flags
			FLAG_PARTFILE		= 1<<1,		//!< Mcf doesnt contain all files (i.e. its a patch)
			FLAG_NONVERIFYED	= 1<<2,		//!< Content of the file hasnt been verifyed and might not be correct
			FLAG_NOTCOMPRESSED  = 1<<3,		//!< Xml and file is not compressed
			FLAG_NOCLEANUP		= 1<<4,		//!< App shouldnt try and clean this MCF (merge it with others)
			FLAG_COURGETTE		= 1<<5,		//!< Uses courgette for diff
		};

		virtual ~MCFHeaderI()=0;

		//! Checks to see if the header is valid
		//!
		//! @return true for valid, false for invalid
		//!
		virtual bool isValid() const =0;

		//! Gets the magic number for the MCF header. Should be LMCF
		//!
		//! @return Magic number as string
		//!
		virtual const char* getMagicNumber() const = 0;

		//! Gets the MCF file version
		//!
		//! @return File version
		//!
		virtual uint8 getFileVer() const = 0;

		//! Gets the item version
		//!
		//! @return Item content version
		//!
		virtual MCFBuild getBuild() const = 0;

		//! Gets the item desura Id
		//!
		//! @return Desura Id
		//!
		virtual uint32 getId() const = 0;

		//! Gets the item desura type
		//!
		//! @return Desura Type
		//!
		virtual uint8 getType() const = 0;

		//! Gets the Xml Start position
		//!
		//! @return Xml offset
		//!
		virtual uint64 getXmlStart() const = 0;

		//! Gets the Xml Length
		//!
		//! @return Xml Length
		//!
		virtual uint32 getXmlSize() const = 0;

		//! Gets the Mcf Flags
		//!
		//! @return Mcf Flags
		//!
		virtual uint8 getFlags() const = 0;

		//! Gets the MCF parent version (i.e. the MCF this one was based off)
		//!
		//! @return Mcf Parent Version
		//!
		virtual uint32 getParent() const = 0;


		//! Sets the Desura type
		//!
		//! @param type Desura type id
		//!
		virtual void setType(uint8 type)=0;

		//! Sets the item version
		//!
		//! @param build Item content version
		//!
		virtual void setBuild(MCFBuild build)=0;

		//! Sets the Desura item Id
		//!
		//! @param id Desura Id
		//!
		virtual void setId(uint32 id)=0;

		//! Sets the MCF parent version (i.e. the one this is based off)
		//!
		//! @param version Patent Version
		//!
		virtual void setParent(uint32 version)=0;

		//! Adds a flag to the MCF
		//!
		//! @param flag Flags to add
		//!
		virtual void addFlags(uint8 flag)=0;

		//! Removes a flag from the MCF
		//!
		//! @param flag Flags to remove
		//!
		virtual void delFlags(uint8 flag)=0;

		//! Compares two Mcf Headers and see if they are the same
		//!
		//! @param other Header to compare against
		//! @return true If matches, false if doesnt
		//!
		virtual bool matches(MCFHeaderI* other) const = 0;

		//! Gets the desura id class
		//!
		//! @return Desura id
		//!
		virtual DesuraId getDesuraId() const = 0;

		//! Gets the MCF branch
		//!
		//! @return Mcf branch
		//!
		virtual MCFBranch getBranch() const = 0;

		//! Sets the MCF branch
		//!
		//! @param branch MCF branch
		//!
		virtual void setBranch(MCFBranch branch)=0;

		//! Changes the file version to the newest
		//!
		virtual void updateFileVersion()=0;

		//! Gets the size of the header for in a MCF file
		//!
		//! @return size of the header in a MCF file
		//!
		virtual uint8 getSize() const = 0;
	};


#ifdef LINK_WITH_GMOCK
	class MCFHeaderMock : public MCFHeaderI
	{
	public:
		MOCK_METHOD0(isValid, bool());
		MOCK_METHOD0(getMagicNumber, const char*());
		MOCK_METHOD0(getFileVer, uint8());
		MOCK_METHOD0(getBuild, MCFBuild());
		MOCK_METHOD0(getId, uint32());
		MOCK_METHOD0(getType, uint8());
		MOCK_METHOD0(getXmlStart, uint64());
		MOCK_METHOD0(getXmlSize, uint32());
		MOCK_METHOD0(getFlags, uint8());
		MOCK_METHOD0(getParent, uint32());
		MOCK_METHOD1(setType, void(uint8 type));
		MOCK_METHOD1(setBuild, void(MCFBuild build));
		MOCK_METHOD1(setId, void(uint32 id));
		MOCK_METHOD1(setParent, void(uint32 version));
		MOCK_METHOD1(addFlags, void(uint8 flag));
		MOCK_METHOD1(delFlags, void(uint8 flag));
		MOCK_METHOD1(matches, bool(MCFHeaderI* other));
		MOCK_METHOD0(getDesuraId, DesuraId());
		MOCK_METHOD0(getBranch, MCFBranch());
		MOCK_METHOD1(setBranch, void(MCFBranch branch));
		MOCK_METHOD0(updateFileVersion, void());
		MOCK_METHOD0(getSize, uint8());
	};
#endif


}

#endif
