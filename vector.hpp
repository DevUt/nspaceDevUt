#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <initializer_list>
#include <compare>
#include <stdexcept>
#include <new>
namespace Ut{
	template<class vector>
	class vector_iterator{
		public :
			using value_type = typename vector::value_type;
			using pointer_type = value_type*;
			using reference_type = value_type&;
		private :
			pointer_type base_ptr;
		public :
			vector_iterator(pointer_type ptr) : base_ptr(ptr) {};
			vector_iterator& operator++(){
				base_ptr++;
				return *this;
			}
			vector_iterator operator++(int){
				vector_iterator temp = *this;
				++(*this);
				return temp;
			}
			vector_iterator& operator--(){
				base_ptr--;
				return *this;
			}
			vector_iterator operator--(int){
				vector_iterator temp = *this;
				--(*this);
				return temp;
			}

			template<typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
			const reference_type operator[](I pos) const{
				return base_ptr[pos];
			}
			template<typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
			reference_type operator[](I pos){
				return base_ptr[pos];
			}
			pointer_type operator->() const{
				return base_ptr;
			}
			reference_type operator*() const{
				return *base_ptr;
			}
			auto operator<=> (const vector_iterator& other) const = default;
	};
	template<typename T>
	class vector{
		T *ptr = nullptr;
		size_t _size=0;
		size_t _capacity=0;
		void check_and_double_capacity(){
			if(size() == capacity())
				reserve(capacity()*2);
		}
		public :
			using value_type = T;
			using reference_type = value_type&;
			using pointer_type = value_type*;
			using iterator = vector_iterator<vector<T>>;
		public :
		bool empty() const {
			return (size() == 0);
		}
		reference_type back(){
			return ptr[size()-1];
		}
		const reference_type back() const{
			return ptr[size()-1];
		}
		reference_type front(){
			return ptr[0];
		}
		const reference_type front() const{
			return ptr[0];
		}
		size_t capacity() const{
			return _capacity;
		}
		size_t size() const{
			return _size;
		}

		friend void swap(vector<T>& a, vector<T>& b){
			a.swap(b);
		}
		void clear(){
			resize(0);
		}
		template<typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
		void insert(T&& data, I pos){
			check_and_double_capacity();
			resize(size()+1);
			for(size_t i=pos+1;i<size();i++){
				std::swap(ptr[pos],ptr[pos-1]);
			}
			std::swap(ptr[pos],data);

		}
		template<typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
		void insert(T& data, I pos){
			check_and_double_capacity();
			resize(size()+1);
			for(size_t i=pos+1;i<size();i++){
				std::swap(ptr[pos],ptr[pos-1]);
			}
			ptr[pos] = data;

		}
		template<typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
		void erase(I pos){
			for(size_t i = pos; i<size()-1;i++){
				std::swap(ptr[pos],ptr[pos+1]);
			}
			resize(size()-1);
		}
		vector<T>(size_t _init_size){
			resize(_init_size);
		}
		vector<T>():_size(0),_capacity(2){
			reserve(capacity());
		}
		vector<T>(const vector<T>& orignal){
			reserve(orignal.capacity());
			resize(orignal.size());
			std::copy(orignal.ptr,orignal.ptr+_size,ptr);
		}
		vector<T>(std::initializer_list<T>& l){
			resize(l.size());
			std::copy(l.begin(),l.end(),ptr);
		}
		vector<T>(std::initializer_list<T>&& l){
			resize(l.size());
			std::move(l.begin(),l.end(),ptr);
		}
		vector<T>& operator=(vector<T> copy){
			copy.swap(*this);
			return *this;
		}
		auto operator== (const vector<T>& other) const{
			if(other.size() == (*this).size()){
				for(size_t i =0;i<other.size();i++){
					if(other[i] != (*this)[i])
						return false;
				}
				return true;
			}
			return false;
		}
		template<typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
		const reference_type operator[](const I pos) const{
			return ptr[pos];
		}
		template<typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
		reference_type operator[](const I pos){
			return ptr[pos];
		}
		~vector<T>(){
			resize(0);
		}
		template<typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
		const reference_type at(const I pos) const{
			if(pos >= size())
				throw std::out_of_range(std::to_string(pos) + " is outside the range 0 to " + std::to_string(size()));
			return ptr[pos];
		}
		template<typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
		reference_type at(const I pos){
			if(pos >= size())
				throw std::out_of_range(std::to_string(pos) + " is outside the range 0 to " + std::to_string(size()));
			return ptr[pos];
		}
		void resize(size_t new_size){
			if(new_size > capacity()){
				reserve(new_size);
			}
			if(new_size < size()){
				for(size_t i = new_size;i<size();i++)
					ptr[i].~T();
			}else if(new_size > size()){
				for(size_t i=size();i<new_size;i++)
					new (ptr + i) T();
			}
			_size = new_size;
		}
		template<bool cap_check>
		void reserve_helper(size_t new_cap){
			if constexpr(cap_check)
				if(new_cap < capacity())
					return;
			/*
				if directly casted to (T*)
				made sure to do pointer alignment
				it wouldn't work on platforms where
				breaking alignment is not supported
			*/
			T *new_ptr = (T*) new (std::align_val_t(alignof(T))) std::byte[new_cap*sizeof(T)];
			_capacity = new_cap;
			if(!empty()){
				if constexpr(std::is_trivially_copyable_v<T>)
					std::copy(ptr,ptr+size(),new_ptr);
			}
			else{
				// notes :
				// assigning to memory that isn't alive is UB,
				// so we initiallize with new instead
				for(size_t i=0;i<size();i++)
					new (new_ptr + i) T(std::move(ptr[i]));
			}
			for(size_t i=0;i<size();i++)
				ptr[i].~T();
			::operator delete []((std::byte *)ptr, std::align_val_t(alignof(T)));
			ptr = new_ptr;
		}
		void reserve(size_t new_cap){
			reserve_helper<true>(new_cap);
		}
		void push_back(const T& data){
			check_and_double_capacity();
			resize(size()+1);
			ptr[size()-1] = data;
		}
		void push_back(T&& data){
			check_and_double_capacity();
			resize(size()+1);
			std::swap(ptr[size()-1],data);
		}
		void pop_back(){
			resize(size()-1);
		}
		void swap(vector<T>& rhs){
			std::swap(ptr,rhs.ptr);
			std::swap(_size,rhs._size);
			std::swap(_capacity,rhs._capacity);
		}
		void shrink(){
			reserve_helper<false>(size());
		}
		iterator begin(){
			return iterator(ptr);
		}
		iterator end(){
			return iterator((ptr+size()));
		}
	};
}

// to learn 1. copy and swap idiom
// to learn about how to implement emplace_back and emplace