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

#ifndef DESURA_COMMONParameter_H
#define DESURA_COMMONParameter_H
#ifdef _WIN32
#pragma once
#endif

#include <typeinfo>
#include "IPCParameterI.h"
#pragma warning( disable : 4800)

#define REGTYPE( class, type ) IPC::IPCParameterI* new_##type (){return new class();}IPC::AutoReg<type> ar_##type = IPC::AutoReg<type>( new_##type );IPC::IPCParameterI* newParameterMacro(type val){return new class(val);}
#define REGTYPEP( class, type ) IPC::IPCParameterI* new_##type##p (){return new class();}IPC::AutoReg<type*> ar_##type##p = IPC::AutoReg<type*>( new_##type##p );IPC::IPCParameterI* newParameterMacro(type* val){return new class(val);}
#define REGTYPEC( class, type, name ) IPC::IPCParameterI* new_##name (){return new class();}IPC::AutoReg<type*> ar_##name = IPC::AutoReg<type*>( new_##name );IPC::IPCParameterI* newParameterMacro(type* val){return new class(val);}

#define REGTYPET( class, type, name ) IPC::IPCParameterI* new_##name (){return new class();}IPC::AutoReg<type> ar_##name = IPC::AutoReg<type>( new_##name );IPC::IPCParameterI* newParameterMacro(type val){return new class(val);}



//This is here so the extern escapes the namespace
template <typename T>
IPC::IPCParameterI* newParameterS(T t)
{
	extern IPC::IPCParameterI* newParameterMacro(T);
	return newParameterMacro((T)t);
}




namespace IPC
{
	typedef union
	{
		uint64 u;
		double d;
	} DoubleAndUint64;

	template <typename T>
	uint32 getType()
	{
		return UTIL::MISC::RSHash_CSTR(typeid(T).name());
	}

	typedef union
	{
		char data[4];
		int32 i;
	} chartoi;

	typedef union
	{
		char data[4];
		uint32 i;
	} chartoui;


	inline void Uint32ToBuff(char* buffer, uint32 val)
	{
		chartoui c;
		c.i = val;

		buffer[0] = c.data[0];
		buffer[1] = c.data[1];
		buffer[2] = c.data[2];
		buffer[3] = c.data[3];
	}

	inline uint32 buffToUint32(const char* buffer)
	{
		chartoui c;

		c.data[0] = buffer[0];
		c.data[1] = buffer[1];
		c.data[2] = buffer[2];
		c.data[3] = buffer[3];

		return  c.i;
	}

	inline int32 buffToInt32(const char* buffer)
	{
		chartoi c;

		c.data[0] = buffer[0];
		c.data[1] = buffer[1];
		c.data[2] = buffer[2];
		c.data[3] = buffer[3];

		return  c.i;
	}



	class PVoid : public IPCParameterI
	{
	public:
		PVoid();
		PVoid(uint64 v){}

		uint32 getSerializeSize() override;
		void serialize(char* szBuffer) override;
		uint32 deserialize(const char* buffer, uint32 size);
		uint64 getValue(bool dup);

		uint32 getType(){ return PVoid::getTypeS(); }
		static uint32 getTypeS(){ return IPC::getType<void>(); }
	};

	class PBool : public IPCParameterI
	{
	public:
		PBool();
		PBool(bool val);

		uint32 getSerializeSize() override;
		void serialize(char* szBuffer) override;
		uint32 deserialize(const char* buffer, uint32 size);
		uint64 getValue(bool dup);

		uint32 getType(){ return PBool::getTypeS(); }
		static uint32 getTypeS(){ return IPC::getType<bool>(); }

	private:
		bool m_bValue;
	};

	class PUint32 : public IPCParameterI
	{
	public:
		PUint32();
		PUint32(uint32 val);

		uint32 getSerializeSize() override;
		void serialize(char* szBuffer) override;
		uint32 deserialize(const char* buffer, uint32 size);
		uint64 getValue(bool dup);

		uint32 getType(){ return PUint32::getTypeS(); }
		static uint32 getTypeS(){ return IPC::getType<uint32>(); }

	private:
		uint32 m_uiValue;
	};

	class PInt32 : public IPCParameterI
	{
	public:
		PInt32();
		PInt32(int32 val);

		uint32 getSerializeSize() override;
		void serialize(char* szBuffer) override;
		uint32 deserialize(const char* buffer, uint32 size);
		uint64 getValue(bool dup);

		uint32 getType(){ return PInt32::getTypeS(); }
		static uint32 getTypeS(){ return IPC::getType<int32>(); }

	private:
		int32 m_iValue;
	};

	class PUint64 : public IPCParameterI
	{
	public:
		PUint64();
		PUint64(uint64 val);

		uint32 getSerializeSize() override;
		void serialize(char* szBuffer) override;
		uint32 deserialize(const char* buffer, uint32 size);
		uint64 getValue(bool dup);

		uint32 getType(){ return PUint64::getTypeS(); }
		static uint32 getTypeS(){ return IPC::getType<uint64>(); }

	private:
		uint64 m_uiValue;
	};


	class PDouble : public IPCParameterI
	{
	public:
		PDouble();
		PDouble(double val);

		uint32 getSerializeSize() override;
		void serialize(char* szBuffer) override;
		uint32 deserialize(const char* buffer, uint32 size);
		uint64 getValue(bool dup);

		uint32 getType(){ return PDouble::getTypeS(); }
		static uint32 getTypeS(){ return IPC::getType<double>(); }

	private:
		double m_dValue;
	};


	class PString : public IPCParameterI
	{
	public:
		PString();
		PString(const char* str);

		uint32 getSerializeSize() override;
		void serialize(char* szBuffer) override;
		uint32 deserialize(const char* buffer, uint32 size);
		uint64 getValue(bool dup);

		uint32 getType(){ return PString::getTypeS(); }
		static uint32 getTypeS(){ return IPC::getType<char*>(); }

	private:
		bool m_bNull = false;
		std::string m_szValue;
	};


	class PException : public IPCParameterI
	{
	public:
		PException();
		PException(gcException& e);
		~PException();

		uint32 getSerializeSize() override;
		void serialize(char* szBuffer) override;
		uint32 deserialize(const char* buffer, uint32 size);
		uint64 getValue(bool dup);

		uint32 getType(){ return PException::getTypeS(); }
		static uint32 getTypeS(){ return IPC::getType<gcException>(); }

	private:
		gcException *exception;
	};



	class PBlob : public IPCParameterI
	{
	public:
		PBlob();
		PBlob(const PBlob& e);
		PBlob(const char* data, uint32 size);
		PBlob(PBlob* blob);
		PBlob(uint64 val);
		~PBlob();

		uint32 getSerializeSize() override;
		void serialize(char* szBuffer) override;
		uint32 deserialize(const char* buffer, uint32 size);
		uint64 getValue(bool dup);

		uint32 getType(){ return PBlob::getTypeS(); }
		static uint32 getTypeS(){ return IPC::getType<PBlob>(); }

		uint32 getSize()const{ return m_uiSize; }
		char* getData()const{ return m_szData; }

	private:
		char* m_szData;
		uint32 m_uiSize;
	};




	class PMapStringString : public IPCParameterI
	{
	public:
		PMapStringString();
		PMapStringString(const std::map<std::string, std::string>& e);
		~PMapStringString();

		uint32 getSerializeSize() override;
		void serialize(char* szBuffer) override;
		uint32 deserialize(const char* buffer, uint32 size);
		uint64 getValue(bool dup);

		uint32 getType(){ return PMapStringString::getTypeS(); }
		static uint32 getTypeS(){ return IPC::getType<PMapStringString>(); }

	private:
		std::map<std::string, std::string> m_Map;
	};










	typedef IPCParameterI* (*newFunc)();

	void RegType(const std::type_info& type, newFunc funct);

	template <typename T>
	class AutoReg
	{
	public:
		AutoReg(newFunc funct)
		{
			RegType(typeid(T), funct);
		}
	};

	IPCParameterI* newParameter(uint32 type, const char* buff, uint32 size);

	template<typename T>
	IPCParameterI* newParameter(T t)
	{
		return newParameterS(t);
	}

	inline void param_delete(const char*& ptr)
	{
	}

	inline void param_delete(const wchar_t*& ptr)
	{
	}

	inline void param_delete(char*& ptr)
	{
		safe_deleteA(ptr);
	}

	inline void param_delete(wchar_t*& ptr)
	{
		safe_deleteA(ptr);
	}

	template <typename T>
	void param_delete(T*& ptr)
	{
		if (ptr)
			delete ptr;
		ptr = nullptr;
	}

	template <typename T>
	void param_delete(T& ptr)
	{
	}

	template <typename T>
	IPCParameterI* getParameter(T base, bool delBase = false)
	{
		IPCParameterI* ret = newParameterS(base);

		if (delBase)
			param_delete(base);

		return ret;
	}

	template <typename T>
	IPCParameterI* getParameter()
	{
		return newParameterS(T());
	}

	inline IPCParameterI* getParameter(uint32 type, const char* buff, uint32 size)
	{
		return newParameter(type, buff, size);
	}

	template <typename R>
	R getParameterValue(IPCParameterI* p, bool dup = false)
	{
		return (R)p->getValue(dup);
	}

	template <>
	inline double getParameterValue<double>(IPCParameterI* p, bool dup)
	{
		DoubleAndUint64 du;
		du.u = p->getValue(dup);
		return du.d;
	}

	template <>
	inline std::map<std::string, std::string> getParameterValue<std::map<std::string, std::string>>(IPCParameterI* p, bool dup)
	{
		std::map<std::string, std::string> res(*reinterpret_cast<std::map<std::string, std::string>*>(p->getValue(false)));
		return res;
	}
}



#endif //DESURA_COMMONParameter_H
