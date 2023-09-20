#include <mavros/mavros_plugin.h>
#include <mavros_msgs/ActuatorOptim.h>

namespace mavros {
namespace extra_plugins {
class OptimResultPlugin:public plugin::PluginBase {
public:
	OptimResultPlugin():PluginBase(),
		nh("~")
	{}

	void initialize(UAS &uas_) override {
		PluginBase::initialize(uas_);

		result_sub = nh.subscribe("Optim_Receive", 14, &OptimResultPlugin::result_cb, this);
	}

	Subscriptions get_subscriptions() override {
		return{};
	}

private:
	ros::NodeHandle nh;
	ros::Subscriber result_sub;

	/* -*- callbacks -*- */

	void result_cb(const mavros_msgs::ActuatorOptim &msg)
	{
		mavlink::common::msg::ACTOPT_RESULT ar;
		//讲server收到的request赋值给mavlink消息
		for (int i = 0; i < 6; i++)
		{
			ar.opt_result[i] = msg.controls[i];
		}
		//响应发送成功
		ar.flag = true;
		//调用mavlink消息发送API
		UAS_FCU(m_uas)->send_message_ignore_drop(ar);
	}
};
}	// namespace extra_plugins
}	// namespace mavros



#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(mavros::extra_plugins::OptimResultPlugin, mavros::plugin::PluginBase)

