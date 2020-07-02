
namespace nexus {

	// ��ɫ��¼�˺���֤
	void nlan_session::handle_login_proof_2(const nmsg_base* packet)
	{
		const tagC2S_LoginProof* data_ptr = static_cast<const tagC2S_LoginProof*>(packet);

		tagS2C_LoginProofResult request;
		request.client_id		= data_ptr->client_id;
		request.error			= ELoginProof_SUCCESS;

#ifdef	NHANDLE_LOGIN_PROOF
		float	elapse_time = get_time();
#endif

#ifdef	NHANDLE_LOGIN_PROOF_NOLY
		float	elapse_time_proof_only = get_time();
#endif
		// ���г�ʼ��֤
		nworld* world_ptr = g_world_mgr.get_world( get_world_id() );
		if( !world_ptr )
		{//��֤world�Ƿ����
			request.error = ELoginProof_WorldName_Wrong;
		}
		else if( !world_ptr->is_well() )
		{//World�Ƿ�����
			request.error = ELoginProof_World_No_Use;
		}
		else if( !g_player_account_mgr.check_version(data_ptr->cur_version) )
		{//��֤�汾���Ƿ�һ��
			request.error = ELoginProof_Wrong_Version;
		}
		//else if( !g_player_account_mgr.exist_account(data_ptr->account_id) )
		//{//����˻��Ƿ����
		//	request.error = ELoginProof_No_Match;
		//}

		if( ELoginProof_SUCCESS != request.error )
		{//��֤�����򷵻ؿͻ���
			send_message(&request, sizeof(request));
		}
		else
		{
			nlogin_player* player_ptr = nlogin_player::alloc_login_player();
			if( player_ptr )
			{
				if( player_ptr->init(data_ptr->client_id, data_ptr->account_id, data_ptr->ip) )
				{
					if( !player_ptr->is_status(EPLS_None) )
					{//�Ѿ���½�����Ѿ�������Ϸ����
						request.error = ELoginError_UNKNOWN;
					}
					else
					{//��ʼ��½
				world_ptr->add_proof_player(player_ptr);
						if( player_ptr->proof(data_ptr->psd) )
						{//��֤�ɹ�
#ifdef	NHANDLE_LOGIN_PROOF
							elapse_time = get_time() - elapse_time;
							NLOG_INFO(_T("%s, the time of proof account and psd  is %f"), __TFUNCTION__, elapse_time);
#endif

#ifdef	NHANDLE_LOGIN_PROOF_NOLY
							elapse_time_proof_only = get_time() - elapse_time_proof_only;
							NLOG_INFO(_T("%s, the time of proof account and psd  is %f"), __TFUNCTION__, elapse_time_proof_only);
							world_ptr->remove_from_proof(player_ptr);
							world_ptr->remove_from_queue(player_ptr);
							nlogin_player::free_login_player(player_ptr);

#endif
							return;
			}
			else
						{//��֤ʧ��
							world_ptr->remove_from_proof(player_ptr);
							request.error = ELoginProof_Psd_Error;
						}
					}
				}
				else
			{
					request.error = ELoginProof_No_Match;
				}
			}
				send_message(&request, sizeof(request));
			nlogin_player::free_login_player(player_ptr);
			}
		}

	void nlan_session::handle_mibao(const nmsg_base* packet)
	{
		const tagC2S_MiBao* data_ptr = static_cast<const tagC2S_MiBao*>(packet);

		nworld* world_ptr = g_world_mgr.get_world(get_world_id());
		if (!world_ptr)
		{
			NLOG_DEBUG(_T("can't find the world!"));
			return;
		}

		nlogin_player* player_ptr = world_ptr->get_proofing_player(data_ptr->client_id);
		if(!player_ptr)
		{
			NLOG_DEBUG(_T("can't find player!"));
			return;
		}

		player_ptr->end_proof_mibao(data_ptr->mibao_crc);
	}

	void nlan_session::handle_account_logout(const nmsg_base* packet)
	{
		const tagMsg_Player_Logout* data_ptr = static_cast<const tagMsg_Player_Logout*>(packet);

		nworld* world_ptr = g_world_mgr.get_world( get_world_id() );
		if (!world_ptr)
		{
			NLOG_DEBUG(_T("can't find the world!"));
			return;
		}

		world_ptr->login_out(data_ptr->account_id);
	}

} // namespace nexus
