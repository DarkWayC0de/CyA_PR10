/**
 * @file GR.h
 * @version 1.0
 * @date 16/12/2018
 * @author Diego Cruz Rodriguez
 * @title Clase Gramatica
 * @brief Clase para trabajar con Gramaticas
 */

#ifndef PR9_GR_H
#define PR9_GR_H

#include <DFA.h>
#include <fstream>
#include "regla.h"
/**
 * @brief Uso del namespace CyA
 */
namespace CyA {
  /**
   * @brief Clase para el uso de Gramaticas
   */
  class GR {
  private:
    std::vector<char> simbolosterminales_;
    std::vector<char> simbolosnoterminales_;
    std::vector<regla> reglas_;
    char calculartermino(unsigned arranque, unsigned actual);
    bool pertenece( char);
    std::vector<char> reglas_anulables(std::vector<char> &);
    void desarollo_eliminacion_anulables(std::vector<char>);
    void desarollo_eliminacion_unitaria(std::vector<char>, GR &A);
    std::vector<std::string> buscar_reglas(char);
    int buscar_simbolo(char);
  public:
    /**
     * @brief Constructor por defecto de GR
     */
    GR ();
    /**
     * @brief Destructor por defecto de GR
     */
    ~GR();
    /**
     * @brief Funcion para cargar una gramatica desde fichero
     * @param nombrefichero Nombre del fichera de la gramatica
     */
    void cargar(char nombrefichero[]);
    /**
     * @brief Funcion para cargar una gramatica desde un dfa
     * @param dfa_ Dfa para cargar la gramatica
     */
    void cargardesdeDFA(DFA dfa_);
    /**
     * @brief Funcion para exportar la gramatica a un fichero
     * @param nombrefichero Nombre del fichero a exportar
     */
    void exportar(char nombrefichero[]);
    /**
     * @brief Imprime una gramaticanateriormente cargada
     * @param os parametro de imprecion
     * @return El parametro de imprecion
     */

    std::ostream& write(std::ostream &os);
    /**
     * @brief Elimina las produciones vacias de la gramatica llamante
     * @return La nueva gramatica
     */
    GR eliminarprvacias();
    /**
     * @brief Elimina las produciones unitarias de la gramatica llamante
     * @return devuelve la nueva gramatica
     */
    GR eliminarprunitarias(void);
    /**
     * @brief Sobrecarga del operador de no igual para GR
     * @param a Gramatica
     * @return boleano  solucion
     */
    int operator!=(const CyA::GR &a) const;
    /**
     * @brief Sobrecarga del operador de igualdad para GR
     * @param a Gramatica
     * @return boleano  solucion
     */
    int operator==(const CyA::GR &a) const;
  };
}


#endif //PR9_GR_H
