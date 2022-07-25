// CppGround.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение
// программы.
//

#include <iostream>


int main()
{
    
	
    return 0;
}

// Делегат 
template<typename ReturnType, typename... FunctionArguments>
struct Delegate
{
private:
    class Dummy
    {};

	// создаём указатель на объект вложенного класса 
    Dummy* object;
	// создаём указатель на не существующий метод вложенного класса "да так можно"
    ReturnType(Dummy::* method)(FunctionArguments...);

public:

	// создаём шаблонную функцию которой передаём тип класса того объекта у которого мы хотим получить метод
    template<typename T>
    void bind(const T& object, ReturnType(T::* function)(FunctionArguments...))
    {
    	// так как мы получили указатель то мы можем его костануть на укозатель нашего вложенного класса 
        this->object = (Dummy*)(&object);

    	// тут происходит жопная магия
    	// сперва мы создаём юнион с двумя объектами тоесть у нас на 2 указателя на функцию будет одна область в памяти 
        union // кстати синтаксис без имени норм мы просто создадим экземпляр сразу и имя будет не нужно 
        {
            ReturnType(T::* uncast)(FunctionArguments...);
            ReturnType(Dummy::* cast)(FunctionArguments...);
        } u;
    	// создав экземпляр в указатель на поле с переданным ранее типом класса присваеваем полученный указатель на метод 
        u.uncast = function;
    	// а за тем в наглую вынимаем эти же данные только преставляем их как данные уже другого типа
    	// пофакту это бронебойный каст 
        this->method = u.cast;
    }

	// ну и после реализуем метод вызова метода, там ебаный синтиксис который мы можем наблюдать в сточке снизу 
    ReturnType invoke(FunctionArguments... args) { return (*object.*(method))(args...); }
};

void kekw()
{
    Delegate<void, int, int, int> d;
}