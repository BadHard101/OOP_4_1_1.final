#include "cl_base.h"

cl_base* cl_base::root = new cl_base();

cl_base::cl_base()
{
	parent = nullptr;
	status = 1;
}

cl_base::cl_base(string object_name, cl_base* parent)
{
	this->object_name = object_name;
	if (parent == nullptr)
	{
		set_parent(root);
		(this->parent)->children.push_back(this);
	}
	else
	{
		this->parent = parent;
		parent->children.push_back(this);
	}
	children.push_back(this);
	index = (this->parent)->children.size() - 1;
}

void cl_base::set_name(string name)
{
	this->object_name = name;
}

void cl_base::set_parent(cl_base* parent)
{
	this->parent = parent;
}

cl_base* cl_base::get_parent()
{
	return parent;
}

string cl_base::get_name()
{
	return object_name;
}

int cl_base::get_status()
{
	return this->status;
}

void cl_base::set_status(int status)
{
	if (parent->get_status() != 0)
		this->status = status;
	if (this->status == 0) {
		this->status = 1;
		for (int i = 1; i < this->children.size(); i++)
			children[i]->set_status(0);
		this->status = 0;
	}
}

cl_base* cl_base::get_object_by_name(string name)
{
	cl_base* val = nullptr;
	for (int i = 0; i < this->children.size(); i++)
	{
		val = children[i];
		if (children[i]->get_name() == name)
			return children[i];
	}

	for (int i = 1; i < children.size(); i++) 
	{
		val = (children[i]->get_object_by_name(name));
		if ((children[i]->get_object_by_name(name))->get_name() == name)
			return (children[i]->get_object_by_name(name));
	}
	return val;
}

cl_base* cl_base::get_child_by_name(string name) //CL_3_2
{
	cl_base* val = nullptr;
	for (int i = 1; i < this->children.size(); i++)
	{
		val = children[i];
		if (children[i]->get_name() == name)
			return children[i];
	}
	return nullptr;
}

//cl_base* cl_base::get_object_by_coord(string path)
//{
//	cl_base* cur = root->children[1];
//	string delimiter1 = "/", delimiter2 = "//", cur_path;
//	cur_path = path;
//	string token;
//	bool path_flag = true;
//
//	size_t pos = 0;
//
//	if (cur_path == "/")
//		return root->children[1];
//	else {
//		while ((pos = cur_path.find(delimiter1)) != string::npos) {
//			token = cur_path.substr(0, pos);
//
//			if (token != "")
//				cur = cur->get_child_by_name(token);
//
//			cur_path.erase(0, pos + delimiter1.length());
//		}
//		cur = cur->get_child_by_name(cur_path);
//	}
//
//	return cur;
//}

cl_base* cl_base::get_object_by_coord(string path) //CL_3_2
{
	cl_base* cur = current;
	string delimiter1 = "/", delimiter2 = "//";
	string cur_path = path, token;

	size_t pos = 0;

	if (cur_path == "/")
		return root->children[1];

	else if (cur_path == ".")
		return cur;

	else if (pos = cur_path.find(delimiter2) < 2) {
		cur_path.erase(0, delimiter2.length());
		cur = root->children[1]->get_object_by_name(cur_path);
	}

	else {
		while ((pos = cur_path.find(delimiter1)) != string::npos) {
			token = cur_path.substr(0, pos);

			if (token == "")
				cur = root->children[1];
			else {
				cur = cur->get_child_by_name(token);
				if (cur == nullptr)
					return cur;
			}

			cur_path.erase(0, pos + delimiter1.length());
		}
		cur = cur->get_child_by_name(cur_path);
	}

	return cur;
}

/*void cl_base::print_tree()
{
	cout << root->children[1]->get_name();
	for (int i = 1; i < children.size(); i++)
	{
		cout << "    ";
		cout << children[i]->get_name();
		if (children[i]->children.size() > 1)
		{
			cout << endl << "    ";
			children[i]->print_tree();
		}
		if (i + 1 < children.size())
			cout << endl;
	}

	for (int i = 1; i < children.size(); i++)
	{
		if (children[i]->children.size() > 1)
		{
			children[i]->print_tree();
		}
	}
}*/

void cl_base::print_tree() 
{
	for (int i = 0; i < children.size(); i++)
	{
		if (i == 0)
			cout << endl;
		cout << children[i]->get_name();
		if (i + 1 < children.size())
			cout << "  ";
	}

	for (int i = 1; i < children.size(); i++)
	{
		if (children[i]->children.size() > 1)
		{
			children[i]->print_tree();
		}
	}
}

void cl_base::print_tree_format(int k) 
{
	for (int i = 1; i < children.size(); i++)
	{
		cout << endl;
		for (int j = 0; j < k; j++)
			cout << "    ";
		cout << children[i]->get_name();
		if (children[i]->children.size() > 1) {
			children[i]->print_tree_format(k + 1);
		}
	}
}

void cl_base::print_tree_status(int k)
{
	for (int i = 1; i < children.size(); i++)
	{
		cout << endl;
		for (int i = 0; i < k; i++)
			cout << "    ";
		cout << children[i]->get_name();

		if (children[i]->get_status() != 0)
			cout << " is ready";
		else
			cout << " is not ready";

		if (children[i]->children.size() > 1) {
			children[i]->print_tree_status(k + 1);
		}
	}
}

string cl_base::get_address()
{
	cl_base* tmp = this;
	bool flag = true;
	if (this->get_parent() == nullptr) return "/";
	string adress;
	while (tmp->get_parent() != nullptr)
	{
		if (tmp->get_name() != root->children[1]->get_name())
		{
			adress.insert(0, tmp->get_name());
			adress.insert(0, "/");
			flag = false;
		}
		else if (flag)
			adress.insert(0, "/");
		
		tmp = tmp->get_parent();
	}
	return adress;
}

void cl_base::set_connect(TYPE_SIGNAL signal_method, cl_base* handler_ob, TYPE_HANDLER handler_method)
{
	signal_handler_connection* tmp = new signal_handler_connection(handler_ob, signal_method, handler_method);
	for (int i = 0; i < connections.size(); i++)
	{
		if (connections[i]->handler_ob == tmp->handler_ob 
			and connections[i]->handler_method == tmp->handler_method
			and connections[i]->signal_method == tmp->signal_method) return;
	}
	connections.push_back(tmp);
}

void cl_base::emit(TYPE_SIGNAL signal_method, string message)
{
	if (this->status==0) return;
	(this->*signal_method)(message);
	for (int i = 0; i < connections.size(); i++)
	{
		if (connections[i]->signal_method == signal_method)
		{
			if (connections[i]->handler_ob->status == 0) continue;
			(connections[i]->handler_ob->*(connections[i]->handler_method))(message);
		}
	}
}

void cl_base::delete_connect(TYPE_SIGNAL signal_method, cl_base* handler_ob, TYPE_HANDLER handler_method)
{
	for (int i = 0; i < connections.size(); i++)
	{
		if (connections[i]->handler_ob == handler_ob 
			and connections[i]->handler_method == handler_method
			and connections[i]->signal_method == signal_method)
		{
			connections.erase(connections.begin() + i);
			return;
		}
	}
}

int cl_base::get_num()
{
	return this->cl_num;
}

cl_base::~cl_base()
{
}