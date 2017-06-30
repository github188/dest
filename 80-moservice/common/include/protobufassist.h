// 提供一些protobuf的辅助接口 by zh. 20150325
#ifndef _GOOGLE_PROTOBUF_ASSIST_
#define _GOOGLE_PROTOBUF_ASSIST_
class PBAssist{
public:
static bool SetFiledData(google::protobuf::Message *message, 
	const ::std::string &field_name, const std::string &data)
{
	const google::protobuf::Reflection *reflection = message->GetReflection();
	int filed_count = message->GetDescriptor()->field_count();
	int index = 0;
	for (index = 0; index < filed_count; index++)
	{
		const google::protobuf::FieldDescriptor *field_descriptor =
			message->GetDescriptor()->field(index);
		if (field_descriptor != NULL &&
			field_descriptor->name() == field_name &&
			!field_descriptor->is_repeated())
		{
			google::protobuf::Message *field_message 
				= reflection->MutableMessage(message, field_descriptor);
			return field_message->ParseFromString(data);
		}
	}
	return false;
}

static bool GetFiledData(const google::protobuf::Message *message, 
	const ::std::string &field_name, std::string &data)
{
	const google::protobuf::Reflection *reflection = message->GetReflection();
	int filed_count = message->GetDescriptor()->field_count();
	int index = 0;
	for (index = 0; index < filed_count; index++)
	{
		const google::protobuf::FieldDescriptor *field_descriptor =
			message->GetDescriptor()->field(index);
		if (field_descriptor != NULL &&
			field_descriptor->name() == field_name &&
			!field_descriptor->is_repeated())
		{
			google::protobuf::Message *field_message =
				reflection->MutableMessage(const_cast<google::protobuf::Message*>(message), 
				field_descriptor);
			return field_message->SerializeToString(&data);
		}
	}
	return false;
}

// 根据proto类名获取实例，由调用者负责释放
static google::protobuf::Message* GetMessageByName(const std::string &msg_name)
{
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor = 
		GetMessageDescByName(msg_name);
	if (descriptor)
	{
		const google::protobuf::Message* defalutInstance = 
			google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (defalutInstance)
		{
			message = defalutInstance->New();
		}
	}
	return message;
}

static const google::protobuf::Descriptor* GetMessageDescByName(const std::string &msg_name)
{
	return google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(msg_name);
}

};
#endif
