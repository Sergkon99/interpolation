#include "../equationsystem.h"


void EquationSystem::init(size_t _n){
    A.resize(_n);
    for (size_t i = 0; i < _n; i++) {
        A[i].resize(_n);
    }
    B.resize(_n);
}

EquationSystem::EquationSystem()
{

}

EquationSystem::EquationSystem(size_t n){
    init(n);
}

EquationSystem::EquationSystem(const vvd& _A, const vd& _B){
    n = _A.size();
    A = _A;
    B = _B;
}
// TODO
EquationSystem::EquationSystem(QFile* file){
    bool isN = true;
    size_t i = 0;

    if(!file->open(QIODevice::ReadOnly)){
        while(!file->atEnd()){
            QString curLine = file->readLine();

            QStringList curList = curLine.split(" ");

            if(isN){
                isN = false;
                n = curList[0].toULong();
                init(n);

                continue;
            }

            for(size_t j = 0; j < n; j++){
                A[i][j] = curList[static_cast<int>(j)].toDouble();
            }
            B[i] = curList[static_cast<int>(i)].toDouble();

            i++;
        }
    }
}

void EquationSystem::read(){
    for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                cin >> A[i][j];
            }
            cin >> B[i];
        }
}

vvd EquationSystem::getMinor(vvd mat, size_t _i, size_t _j){
    size_t sz = mat.size() - 1;
    vvd minor(sz, vd(sz));

    size_t s_i = 0, s_j = 0;

    for (size_t i = 0; i < sz; i++) {
        if (_i == i) {
            s_i = 1;
        }
        for (size_t j = 0; j < sz; j++) {
            if (_j == j) {
                s_j = 1;
            }

            minor[i][j] = mat[i + s_i][j + s_j];

        }
        s_j = 0;
    }

    return minor;
}

double EquationSystem::getDet(const vvd &mat){
    if (mat.size() == 1)
            return mat[0][0];

        int sign = 1;

        double res = 0;

        for (size_t i = 0; i < mat.size(); i++) {
            vvd minor = getMinor(mat, 0, i);
            res += sign * mat[0][i] * getDet(minor);
            sign *= -1;
        }

        return res;
}

int EquationSystem::solve(vd & res){
    return solve(A, B, res);
}

int EquationSystem::solve(const vvd & mat, const vd & ans, vd & res){
    double mainDet = getDet(mat);
    size_t sz = ans.size();
    bool allZero = true;
    bool allNotZero = true;
    res.resize(sz);

    for (size_t i = 0; i < sz; i++){
        vvd tmp = mat;
        for (size_t k = 0; k < sz; k++) {
            tmp[k][i] = ans[k];
        }

        double tmpDet = getDet(tmp);

        res[i] = tmpDet;

        if (abs(res[i]) < eps)allNotZero = false;

        if (abs(res[i]) > eps)allZero = false;
    }

    if (abs(mainDet) < eps) {
        if (allZero) {
            return 1;
        }
        if (allNotZero) {
            return 2;
        }
        return 3;
    }

    for (size_t i = 0; i < sz; i++) {
        res[i] /= mainDet;

        if (abs(res[i]) < eps)
            res[i] = 0;
    }

    return 0;
}

void EquationSystem::printDebug(){
    for (size_t i = 0;i < n; i++) {
        for(size_t j=0;j < n; j++){
            qDebug() << A[i][j]<<" ";
        }
        qDebug() << endl;
    }

    for(size_t i = 0;i < n; i++)
        qDebug() << B[i]<<" ";
}

