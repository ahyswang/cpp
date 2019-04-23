#include <stdio.h>
#include <queue>
#include <set>
#include <fstream>
#include <allocators>
#include <new>
#include "tokenizer_kernel_1.h"
#include "cpp_tokenizer_kernel_1.h"
#include "directed_graph_kernel_1.h"
#include "dlib.h"
#include "graph_kernel_1.h"

using namespace dlib;

directed_graph_kernel_1<std::string, std::string>::edge_type& edge(directed_graph_kernel_1<std::string, std::string> &g, unsigned long parent_idx, 
	unsigned long child_idx) 
{
	for (unsigned long i = 0; i < g.node(parent_idx).number_of_children(); ++i)
	{
		if (g.node(parent_idx).child(i).index() == child_idx)
			return g.node(parent_idx).child_edge(i);
	}
}

template <typename T>
bool search_for_directed_cycles (
    const T& node,
    std::vector<bool>& visited,
    std::vector<bool>& temp
)
{
    if (temp[node.index()] == true)
        return true;

    visited[node.index()] = true;
    temp[node.index()] = true;

    for (unsigned long i = 0; i < node.number_of_children(); ++i)
    {
        if (search_for_directed_cycles(node.child(i), visited, temp))
            return true;
    }
                
    temp[node.index()] = false;

    return false;
}

//深度遍历
template <typename T>
bool search_for_directed_cycles_1 (
	const T& node,
	std::string str
	)
{
	if (node.number_of_children() <= 0) 
	{
		std::cout << str << std::endl;
		return false;
	}
	for (unsigned long i = 0; i < node.number_of_children(); ++i)
	{
		str += node.child_edge(i) + " ";
		search_for_directed_cycles_1(node.child(i), str);
	}
	return true;
}
typedef cpp_tokenizer_kernel_1<tokenizer_kernel_1, std::queue<cpp_tok_kernel_1_helper::token_text_pair>, std::set<std::string>> token_net_type;
typedef directed_graph_kernel_1<std::string, std::string> graph_net_type;
directed_graph_kernel_1<std::string, std::string> g;

enum 
{
	END_OF_FILE,
	KEYWORD,
	COMMENT,
	SINGLE_QUOTED_TEXT,
	DOUBLE_QUOTED_TEXT,
	IDENTIFIER,
	OTHER,
	NUMBER,
	WHITE_SPACE,
	LABEL,
	LABEL_OPTION,
	LABEL_GROUP,
	LABEL_NORMAL,
	LABEL_COMB,
};

int create_edge(graph_net_type &g, unsigned long &idx_0, unsigned long &idx_1, std::string token = "")
{
	idx_0 = g.add_node();
	idx_1 = g.add_node();
	g.add_edge(idx_0, idx_1); edge(g, idx_0, idx_1) = token;
	return 0;
}

int create_net_res(std::string& str, graph_net_type &g, unsigned long &idx_0, unsigned long &idx_1)
{
	token_net_type parse;

	std::istringstream iss(str);
	parse.set_stream(iss);

	int type;
	std::string token;

	bool comb_flag = false;

	idx_0 = g.add_node();
	idx_1 = g.add_node();
	unsigned long idx = idx_0;

	while (true){
		parse.get_token(type, token);
		//std::cout << type << " " << token << "\t";
		if (type == END_OF_FILE) {
			break;
		}
		if (type == LABEL) 
		{
			unsigned long sub0, sub1;
			create_edge(g, sub0, sub1, token);
			g.add_edge(idx, sub0);
			idx = sub1;
		}
		if (type == LABEL_OPTION)
		{
			unsigned long sub0, sub1;
			create_net_res(token, g, sub0, sub1);
			if (sub0 > 0 && sub1 > 0) {
				
				unsigned long e0, e1;
				create_edge(g, e0, e1);
				
				unsigned long op0 = g.add_node();
				unsigned long op1 = g.add_node();
				
				g.add_edge(op0, sub0);
				g.add_edge(op0, e0);
				g.add_edge(sub1, op1);
				g.add_edge(e1, op1);

				g.add_edge(idx, op0);

				idx = op1;
			}
		}
		if (type == LABEL_GROUP)
		{
			unsigned long sub0, sub1;
			create_net_res(token, g, sub0, sub1);
			g.add_edge(idx, sub0);

			idx = sub1;
		}
		if (token == "|")
		{
			g.add_edge(idx, idx_1); //把前一个连接到最后
			idx = idx_0;  //重新付给
		}
	}

	g.add_edge(idx, idx_1);

	return 0;
}

int load_res()
{
	//<querycmd>:[<airpre>][<query>](<querycmd0>|<querycmd1>|<querycmd2>); 
	//普通节点
	//复合节点
	//组合节点
	cpp_tokenizer_kernel_1<tokenizer_kernel_1, std::queue<cpp_tok_kernel_1_helper::token_text_pair>, std::set<std::string>> cpp1;

	std::ifstream fin("test.txt");
	if (!fin)
	{
		std::cout << "open file error!!!" << std::endl;
		return -1;
	}

	cpp1.set_stream(fin);

	int type;
	std::string token;

	while (true){
		cpp1.get_token(type, token);
		std::cout << type << " " << token << "\t";
		if (type == 0) {
			break;
		}
		if (token == ";") {
			std::cout << std::endl;
		}
	}

	return 0;
}


int main(int argc, char **argv)
{
	// 遍历都没有问题
	std::string res = "[<airpre>][<query>](<querycmd0>|<querycmd1>|<querycmd2>)|\
		(<open>|<close>|[<want>])<ac>|\
		<test>; ";
	unsigned long idx0, idx1;
	create_net_res(res, g, idx0, idx1);

	std::string str;

	search_for_directed_cycles_1(g.node(idx0), str);

	//DLIBM_CASSERT(false, "hello");
	//load_res();

	//return 0;
	
// 	g.set_number_of_nodes(9);
// 
// 	g.node(0).data = "0"; 
// 	g.node(1).data = "1";
// 	g.node(2).data = "2";
// 	g.node(3).data = "3";
// 	g.node(4).data = "4";
// 	g.node(5).data = "5";
// 	g.node(6).data = "6";
// 	g.node(7).data = "7";
// 	g.node(8).data = "8";
// 
// 	g.add_edge(0, 1); 
// 	g.add_edge(1, 2);
// 	g.add_edge(1, 4);
// 	g.add_edge(2, 3); edge(g, 2, 3) = "打开";  
// 	g.add_edge(4, 5); edge(g, 4, 5) = "关闭";
// 	g.add_edge(3, 6);
// 	g.add_edge(5, 6);
// 	g.add_edge(6, 7); edge(g, 6, 7) = "空调";
// 
// 	g.add_edge(7, 8); edge(g, 7, 8) = "好不好";
// 
// 	std::vector<bool> vister;
// 	std::vector<bool> temp;
// 
// 	std::string str;
// 
// 	search_for_directed_cycles_1(g.node(0), str);
	
	return 0;
}