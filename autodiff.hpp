#ifndef AUTODIFF_HPP

#define AUTODIFF_HPP


template<typename T>
class Op;

template<typename T>
class Add_Op;

//template<typename T>
//class Add_const_Op;

template<typename T>
class Mul_Op;

//template<typename T>
//class Mul_const_Op;

template<typename T>
class Zeroslike_Op;

template<typename T>
class Oneslike_Op;


#define ZEROSLIKE_ID 0
#define ONESLIKE_ID 1
#define ADD_ID 2
#define MUL_ID 3



#endif // AUTODIFF_HPP
