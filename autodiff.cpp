#include<bits/stdc++.h>
#include<vector>
#include "autodiff.hpp"
using namespace std;


template<typename T>
class Node{
public:
    vector<Node<T>> inputs;
    //
    // do we need a gradient
    //
    T const_var;
    string name;
    //Op<T> *operation;
    int operation_id;

    Node(){
        operation_id = -1;
    };
    //Node(){
    //    operation = new Op<T>(this);
    //};

    ~Node(){
        //if(operation != NULL){
        //    delete operation;
        //    operation = NULL;
        //}
    }

    Node<T> operator +(const Node &x) const{
        return __add__(x);
    }
/*
    Node<T> operator *(const Node &x) const{
        return __mul__(x);
    }
*/
    Node<T> operator *(const Node<T> x) const{
        if(typeid(x) == typeid(Node))
            return __mul__(x);
        //else
        //    return __mul__const(x);
    }

    /*
    Node<T> & operator =(const Node &x){
        if(this != &x){
            inputs.clear();
            for(auto newinput: x.inputs)
                inputs.push_back(newinput);
            operation_id = x.operation_id;
        }
        return *this;
    }
    */


    bool operator <(const Node &x) const{
        return name < x.name;
    }

    Node<T> __add__(const Node<T> &x) const{
        Op<T> *temp_op = new Add_Op<T>();
        auto ret = temp_op->__call__(*this, x);
        delete temp_op;
        return ret;
    }

    Node<T> __mul__(const Node<T> &x) const{
        Op<T> *temp_op = new Mul_Op<T>();
        auto ret = temp_op->__call__(*this, x);
        delete temp_op;
        return ret;
    }

    Op<T>* get_operation() const{
        Op<T> *temp_op = NULL;
        switch(operation_id){
        case ZEROSLIKE_ID:
            temp_op = new Zeroslike_Op<T>();
            break;
        case ONESLIKE_ID:
            temp_op = new Oneslike_Op<T>();
            break;
        case ADD_ID:
            temp_op = new Add_Op<T>();
            break;
        case MUL_ID:
            temp_op = new Mul_Op<T>();
            break;
        default:
            temp_op = NULL;
            break;
        }
        return temp_op;
    }
    T compute(Node<T> x, vector<T> inputs){
        Op<T> *temp_op = get_operation();
        T ret;
        if(temp_op != NULL){
             ret = temp_op->compute(x, inputs);
             delete temp_op;
        }
        return ret;
    };

    vector<Node<T>> gradient(Node<T> x, Node<T> output_grad){
        Op<T> *temp_op = get_operation();
        vector<Node<T>> ret;
        if(temp_op != NULL){
            ret = temp_op->gradient(x, output_grad);
            delete temp_op;
        }
        return ret;
    }
};

template<typename T>
ostream& operator<<(ostream& out,const Node<T> &n){
    return out<<n.name;
}

template<typename T>
class Op{
public:
    Op(){};
    Op(Node<T>* px){
        px->operation = this;
    }
    virtual Node<T> __call__(Node<T> a, Node<T> b){};
    virtual T compute(Node<T> x, vector<T> inputs){};
    virtual vector<Node<T>> gradient(Node<T> x, Node<T> output_grad){};
};

template<typename T>
class Add_Op: public Op<T>{
public:
    Add_Op(){};
    //Add_Op(Node<T>* px){
    //    px->operation = this;
    //}

    Node<T> __call__(Node<T> a, Node<T> b){
        Node<T> newnode;
        newnode.inputs.push_back(a);
        newnode.inputs.push_back(b);
        newnode.name = "(" + a.name + "+" + b.name + ")";
        //newnode.operation = this;
        newnode.operation_id = ADD_ID;
        return newnode;
    };

    //template<typename T>
    T compute(Node<T> x, vector<T> inputs){
        int len = inputs.size();
        assert(len == 2);
        return inputs[0] + inputs[1];
    };

    vector<Node<T>> gradient(Node<T> x, Node<T> output_grad){
        vector<Node<T>> grads;
        grads.push_back(output_grad);
        grads.push_back(output_grad);
        return grads;
    }
};

template<typename T>
class Mul_Op: public Op<T>{
public:
    Mul_Op(){};

    Node<T> __call__(Node<T> a, Node<T> b){
        Node<T> newnode;
        newnode.inputs.push_back(a);
        newnode.inputs.push_back(b);
        newnode.name = "(" + a.name + "*" + b.name + ")";
        //newnode.operation = this;
        newnode.operation_id = MUL_ID;
        return newnode;
    }

    T compute(Node<T> x, vector<T> inputs){
        int len = inputs.size();
        assert(len == 2);
        return inputs[0] * inputs[1];
    };

    vector<Node<T>> gradient(Node<T> x, Node<T> output_grad){
        vector<Node<T>> grads;
        grads.push_back(x.inputs[1] * output_grad);
        grads.push_back(x.inputs[0] * output_grad);
        return grads;
    }
};

/*
template<typename T>
class Mul_const_Op: public Op<T>{
public:
    Mul_const_Op(){};
    Node<T> __call__(Node<T> a, T b){
        Node<T> newnode;
        newnode.inputs.push_back(a);
        newnode.inputs.push_back(b);
        newnode.name = "(" + a.name + "*" + b.name + ")";
        //newnode.operation = this;
        newnode.operation_id = 3;
        return newnode;
    }

    T compute(Node<T> x, vector<T> inputs){
        int len = inputs.size();
        assert(len == 2);
        return inputs[0] * inputs[1];
    };

    vector<Node<T>> gradient(Node<T> x, Node<T> output_grad){
        vector<Node<T>> grads;
        grads.push_back(x.inputs[1] * output_grad);
        grads.push_back(x.inputs[0] * output_grad);
        return grads;
    }

};
*/

template<typename T>
class Placeholder_Op: public Op<T>{
public:
    Placeholder_Op(){};

    Node<T> __call__(string name){
        Node<T> newnode;
        //newnode.operation = this;
        newnode.name = name;
        return newnode;
    }

    T compute(Node<T> x, vector<T> inputs){};
    vector<Node<T>> gradient(Node<T> x, Node<T> output_grad){};
};

template<typename T>
Node<T> Variable(string name){
    return Placeholder_Op<T>().__call__(name);
}

template<typename T>
class Zeroslike_Op: public Op<T>{
public:
    Zeroslike_Op(){};

    Node<T> __call__(Node<T> x){
        Node<T> newnode;
        newnode.inputs.push_back(x);
        newnode.operation_id = ZEROSLIKE_ID;
        //newnode.operation = this;
        newnode.name = "Zeroslike(" + x.name + ")";
        return newnode;
    }

    T compute(Node<T> x, vector<T> inputs){
        return T(0);
    }

    vector<Node<T>> gradient(Node<T> x, Node<T> output_grad){
        vector<Node<T>> grads;
        grads.push_back(__call__(x));
        return grads;
    }
};

template<typename T>
class Oneslike_Op: public Op<T>{
public:
    Oneslike_Op(){};

    Node<T> __call__(Node<T> x){
        Node<T> newnode;
        newnode.inputs.push_back(x);
        newnode.operation_id = ONESLIKE_ID;
        //newnode.operation = this;
        newnode.name = "Oneslike(" + x.name + ")";
        cout<<"in call: "<<newnode<<endl;
        return newnode;
    }

    T compute(Node<T> x, vector<T> inputs){
        //cout<<"suan ni hen"<<endl;
        return T(1);
    };

    vector<Node<T>> gradient(Node<T> x, Node<T> output_grad){
        //cout<<"gradient in ones_like"<<endl;
        vector<Node<T>> grads;
        grads.push_back(Zeroslike_Op<T>().__call__(x));
        return grads;
    };
};

template<typename T>
class Executor{
public:
    //注意：需要这样一个一直存在的实例，不能在gradient里面创建临时实例
    //否则出来后，输出梯度Oneslike(y) 的算子指针就野了, 虽然这样不太elegent
    //Oneslike_Op<T> oppo;
    Executor(){};

    vector<T> forward_run(vector<Node<T>> node_list, map<Node<T>, T> &mp){
        cout<<"begin forward"<<endl;
        vector<Node<T>> topo_rank = topo_sort(node_list);

        for(auto x: topo_rank){
            vector<T> cur_inputs;
            if(x.inputs.size() == 1){
                cur_inputs.push_back(mp[x.inputs[0]]);
                //cout<<"cur caling: "<<x<<" from inputs: "<<mp[x.inputs[0]]<<endl;
            }
            else if(x.inputs.size() == 2){
                cur_inputs.push_back(mp[x.inputs[0]]);
                cur_inputs.push_back(mp[x.inputs[1]]);
                //cout<<"cur caling: "<<x<<" from inputs: "<<mp[x.inputs[0]]<<" and "<<mp[x.inputs[1]]<<endl;
            }
            else{
                //leaf node
                continue;
            }
            //mp[x] = x.operation->compute(x, cur_inputs);
            mp[x] = x.compute(x, cur_inputs);
            //cout<<"res: "<<mp[x]<<endl;
        }
        vector<T> ret;
        for(auto x: node_list)
            ret.push_back(mp[x]);
        return ret;
    }

    vector<Node<T>> build_graph(Node<T> output_node, vector<Node<T>> raw_nodes){
        //compute the gradient graph for raw_nodes
        vector<Node<T>> grad_nodes;
        map<Node<T>, Node<T>> mp;
        //这里很关键???
        Oneslike_Op<T> op;
        //mp[output_node] = oppo.__call__(output_node);
        mp[output_node] = op.__call__(output_node);
        //only single node supported, can be extended by passing vector<Node<T>> output_nodes to build graph
        vector<Node<T>> topo_rank_rev = topo_sort(vector<Node<T>> {output_node});
        reverse(topo_rank_rev.begin(), topo_rank_rev.end());

        cout<<topo_rank_rev.size()<<endl;

        //cout<<"Begin backprop"<<endl;
        for(auto x: topo_rank_rev){
            //Perform 3 steps:
            //1) sum all the gradients from adjacent nodes
            //2) cal sub-gradients for the inputs
            //3) add current node to result

            //leaf node have no children
            if(x.inputs.size() == 0)
                continue;

            Node<T> x_grads = mp[x];
            vector<Node<T>> child_grads = x.gradient(x, x_grads);

            for(int i = 0; i < x.inputs.size(); i++){
                auto child = x.inputs[i];
                if(mp.find(child) == mp.end()){
                    mp[child] = child_grads[i];
                }
                else{
                    mp[child] = mp[child] + child_grads[i];
                }
            }
        }

        for(auto x: raw_nodes){
            grad_nodes.push_back(mp[x]);
        }
        return grad_nodes;
    }

    vector<Node<T>> topo_sort(vector<Node<T>> topo_nodes){
        vector<Node<T>> topo_rank;
        //currently, the node_list begins with the outputs
        set<Node<T>> vis;
        for(auto x: topo_nodes){
            topo_dfs(x, vis, topo_rank);
        }
        return topo_rank;
    }

    void topo_dfs(Node<T> x, set<Node<T>> & vis, vector<Node<T>> &topo_rank){
        if(vis.find(x) != vis.end()) return ;
        vis.insert(x);
        for(auto childs: x.inputs){
            topo_dfs(childs, vis, topo_rank);
        }
        topo_rank.push_back(x);
    }
};

int main(){
    Node<int> x1 = Variable<int>("x1");
    Node<int> x2 = Variable<int>("x2");
    Node<int> y = x1 * x1 + x1 * x2;

    //x2.operation->compute(x2, vector<int>{});
    //Node z = y + y;

    //vector<Node<int>> v={y, x1, x2};
    //Executor<int> e(v);
    cout<<"Ready to build graph"<<endl;
    Executor<int> e;
    vector<Node<int>> grads = e.build_graph(y, vector<Node<int>> {x1, x2});
    //cout<<"check grads"<<endl;
    //for(auto x : grads)
    //    cout<<x<<endl;

    vector<Node<int>> v_cal;
    v_cal.push_back(y);
    for(auto x: grads)
        v_cal.push_back(x);

    map<Node<int>, int> mp;
    mp[x1] = 30;
    mp[x2] = 10;


    vector<int> grad_results = e.forward_run(v_cal, mp);
    for(int i = 0; i < v_cal.size(); i++){
        cout<<v_cal[i]<<' '<<grad_results[i]<<endl;
    }
    cout<<"Done"<<endl;

    return 0;
}
