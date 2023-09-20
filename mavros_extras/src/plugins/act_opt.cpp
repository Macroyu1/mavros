#include <mavros/mavros_plugin.h>
#include <mavros_msgs/ActuatorOptim.h>

namespace mavros {
namespace extra_plugins {
class ActuatorOptimPlugin:public plugin::PluginBase {
public:
	ActuatorOptimPlugin():PluginBase(),
		get_data_nh("~")
	{}

	void initialize(UAS &uas_) override {
		PluginBase::initialize(uas_);
			
		get_data_pub = get_data_nh.advertise<mavros_msgs::ActuatorOptim>("Optim_Receive", 12);
	}

	Subscriptions get_subscriptions() override {
		return{
			make_handler(&ActuatorOptimPlugin::handle_get_data)
		};
	}

private:
	ros::NodeHandle get_data_nh;
	ros::Publisher get_data_pub;
	ros::Subscriber result_sub;
	
	/* -*- rx handlers -*- */
    //mavlink::common::msg::ACTOPT为自动生成的消息头文件中所定义的，也是依据此来解析收到的mavlink消息。
	void handle_get_data(const mavlink::mavlink_message_t *msg, mavlink::common::msg::ACTOPT &get_data) {
		auto tmp = boost::make_shared<mavros_msgs::ActuatorOptim>();
		tmp->flag = get_data.flag;
		for (int i = 0; i < 12; i++)
		{
			tmp->controls[i] = get_data.controls[i];
		}	
		//将解析到的消息发布至topic
		get_data_pub.publish(tmp);//将解析到的消息发布至topic
	}
};
}	// namespace extra_plugins
}	// namespace mavros



#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(mavros::extra_plugins::ActuatorOptimPlugin, mavros::plugin::PluginBase)

