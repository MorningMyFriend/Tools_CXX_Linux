//
// Created by wurui on 18-6-13.
//

#ifndef ALGORITHM_UNION_H
#define ALGORITHM_UNION_H

#include <iostream>
#include <vector>

namespace wrz {

    using namespace std;

// weighted quick union
// maintain a balanced tree, N-length-objs find cost is o(lg2N)

    template<class T>
    class Union {
    public:
        Union() = default;

        Union(const vector<T> &objects) :
                _objs(objects),
                _rootIds(vector<int>(objects.size(), 0)),
                _weights(vector<int>(objects.size(), 1)) {
            for (int i = 0; i < _objs.size(); ++i) {
                _rootIds[i] = i;
            }
        }

        T &operator[](int index) {
            return _objs[index];
        }

        T at(int index) {
            return _objs.at(index);
        }

        void Append(T &obj) {
            _objs.push_back(obj);
            _rootIds.push_back(_rootIds.size());
            _weights.push_back(_weights.size());
        }

        int QuickFind(int index) {
            int i = index;
            int rootId = _rootIds[i];
            while (rootId != _rootIds[i]) {
                _rootIds[i] = _rootIds[rootId];
                i = rootId;
                rootId = _rootIds[i];
            }
            return rootId;
        }

        bool QuickUnion(int id1, int id2, bool useIndex = true) {
            int i = QuickFind(id1);
            int j = QuickFind(id2);
            if (useIndex) {
                if (_weights[i] < _weights[j]) {
                    _rootIds[i] = j;
                    _weights[j] += _weights[i];
                } else {
                    _rootIds[j] = i;
                    _weights[i] += _weights[j];
                }

            } else {
                return false;
            }
        }

        bool IsConnected(int i, int j, bool useIndex = true) {
            if (useIndex) {
                return QuickFind(i) == QuickFind(j);
            } else {
                return false;
            }
        }

        // 显示和第i个元素相连的元素
        void DisplayPath(int i) {
            int index = i;
            int rootId = _rootIds[index];
            cout << _objs[index] << "--";
            while (index != rootId) {
                index = _rootIds[index];
                rootId = _rootIds[index];
                cout << _objs[index] << "--";
            }
        }

    private:
        vector<T> _objs;
        vector<int> _rootIds;
        vector<int> _weights;
    };

}

#endif //ALGORITHM_UNION_H
