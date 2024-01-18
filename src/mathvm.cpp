#include "mathvm.h"
#include "mathModel/exceptions.h"
#include "mathModel/expression/exceptions.h"
#include "mathModel/mfp/exceptions.h"

mathVM::mathVM(std::string scripts_folder)
{
    try {
        model = std::make_unique<mathModel>(scripts_folder);
    }
    catch(failToImport exc) {
        m_excMap = {{"type", "failToImport"}, {"message", exc.what()}};
        emit modelExceptionEvent(getCurExc());
    }
}

void mathVM::setExpression(QString new_expr) {
    m_expression = new_expr;
    m_excMap.clear();
}

QString mathVM::stripTrailingZeroes(QString number) {
    int counter = 0;
    for (auto iter = number.rbegin (); number.indexOf('.') != -1 && (*iter=='0' || *iter=='.'); iter++) {
        counter ++;
        if (*iter == '.') break;
    }
    number = number.chopped(counter);
    return number;
}

QString mathVM::calcIt() {
    try {
        double result = model->calculate_expression(m_expression.toStdString());
        return stripTrailingZeroes(QString::number(result, 'f'));
    }
    // unknownToken, notEnoughArgs, noOpeningPar, invalidChar
    catch (unknownToken exc) {
        m_excMap = {{"type", "unknownToken"},
                  {"message", exc.what()},
                  {"pos", QString::number(exc.pos, 10)}
                 };
        emit modelExceptionEvent(getCurExc());
        return "Error";
    }
    catch (incorrectExpression exc) {
        m_excMap = {{"type", "emptyExpr"},
                  {"message", "Задано пустое или некорректное выражение"},
                 };
        emit modelExceptionEvent(getCurExc());
        return "Error";
    }
    catch (notEnoughArgs exc) {
        m_excMap = {{"type", "notEnoughArgs"},
                  {"message", exc.what()},
                  {"pos", QString::number(exc.pos, 10)}
                 };
        emit modelExceptionEvent(getCurExc());
        return "Error";
    }
    catch (noOpeningPar exc) {
        m_excMap = {{"type", "noOpeningPar"},
                  {"message", exc.what()},
                  {"pos", QString::number(exc.pos, 10)}
                 };
        emit modelExceptionEvent(getCurExc());
        return "Error";
    }
    catch (invalidChar exc) {
        m_excMap = {{"type", "invalidChar"},
                  {"message", exc.what()},
                  {"pos", QString::number(exc.pos, 10)}
                 };
        emit modelExceptionEvent(getCurExc());
        return "Error";
    }
    catch (badReturnType exc){
        m_excMap = {{"type", "badReturnType"}, {"message", exc.what()}};
        emit modelExceptionEvent(getCurExc());
        return "Error";
    }
    catch (invalidOperation exc) {
        m_excMap = {{"type", "invalidOperation"}, {"message", exc.what()}};
        emit modelExceptionEvent(getCurExc());
        return "Error";
    }
    catch (zeroDivision exc) {
        m_excMap = {{"type", "zeroDivision"}, {"message", exc.what()}, {"pos", QString::number(exc.pos, 10)}};
        emit modelExceptionEvent(getCurExc());
        return "Error";
    }
    catch (...) {
        m_excMap = {{"type", "unknown"}, {"message", "При выполнении расчетов произошла неизвестная ошибка"}};
        emit modelExceptionEvent(getCurExc());
        return "Error";
    }
}
