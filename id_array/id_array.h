#pragma once

#include <stdint.h>
#include <stdexcept>
#include <set>

namespace wv
{
	template<typename _Hty, typename _Ty>
	class id_array
	{
	public:
		id_array() { }
		id_array( const size_t& _maxNumHandles ) : m_maxNumHandles{ _maxNumHandles } { }

		~id_array() 
		{
			clear();

			if( m_pBuffer )
				free( m_pBuffer );

			m_pBuffer = nullptr;
			m_size = 0;
			m_maxNumHandles = 0;
		}

		void setMaxNumHandles( const size_t& _maxNumHandles )
		{
			if( _maxNumHandles < m_handles.size() )
				return;
			m_maxNumHandles = _maxNumHandles;
		}

		template<typename...Args>
		[[nodiscard]] _Hty emplace( const Args&... _args );
		[[nodiscard]] _Ty& get( const _Hty& _handle );

		void erase( const _Hty& _handle );
		void clear();
		
		size_t count( void ) { return m_handles.size(); }
		size_t size ( void ) { return m_size; }

	private:

		std::set<_Hty> m_handles;
		uint8_t* m_pBuffer = nullptr;
		size_t m_size = 0;
		size_t m_maxNumHandles = 0;

	};

	template<typename _Hty, typename _Ty>
	template<typename... Args>
	inline _Hty id_array<_Hty, _Ty>::emplace( const Args&... _args )
	{
		if( m_maxNumHandles != 0 && m_handles.size() >= m_maxNumHandles )
			throw std::runtime_error( "id_array is full" );

		uint64_t key = 1;
		while( m_handles.find( (_Hty)key ) != m_handles.end() )
			key++;

		uint64_t index = key - 1; // index is handle-1 because handle 0 is invalid/none
		size_t endOffset = sizeof( _Ty ) * index + sizeof( _Ty );

		if( endOffset >= m_size )
		{
			uint8_t* newptr = (uint8_t*)realloc( m_pBuffer, endOffset );

			if( newptr == nullptr )
				throw std::runtime_error( "failed to reallocate buffer" );
			
			m_pBuffer = newptr;
			m_size = endOffset;
		}

		_Ty* base = &( (_Ty*)m_pBuffer )[ index ];
		_Ty* obj = new( base ) _Ty( _args... );

		m_handles.insert( (_Hty)key );

		return (_Hty)key;
	}

	template<typename _Hty, typename _Ty>
	inline _Ty& id_array<_Hty, _Ty>::get( const _Hty& _handle )
	{
		if( m_handles.find( _handle ) == m_handles.end() )
			throw std::out_of_range( "Invalid _Hty" );

		size_t index = _handle - 1;

		return ( (_Ty*)m_pBuffer )[ index ];
	}

	template<typename _Hty, typename _Ty>
	inline void id_array<_Hty, _Ty>::erase( const _Hty& _handle )
	{
		if( m_handles.find( _handle ) == m_handles.end() )
			throw std::out_of_range( "Cannot erase Invalid _Hty" );

		m_handles.erase( _handle );

		size_t index = _handle - 1;

		_Ty& obj = ( (_Ty*)m_pBuffer )[ index ];
		obj.~_Ty();

		memset( &obj, 0, sizeof( _Ty ) );
	}

	template<typename _Hty, typename _Ty>
	inline void id_array<_Hty, _Ty>::clear()
	{
		for( auto& handle : m_handles )
			erase( handle );
		m_handles.clear();
	}

}