/*
	AI:
		1.	�{�[����ǂ�
		2.	�S�[���Ɍ�����(�{�[���������Ă���)
		3.	�e�L��T��(������)
		4.	�U��(�G������)
		5.	�G�l���M�[�`���[�W
		6.	�^�[�Q�b�g��ǂ�������
		7.	�{�[���������Ă���^�[�Q�b�g��ǂ�
		8.	�{�[���������Ă��閡����ǂ�
		9.	�������痣��Ȃ��悤�Ƀ^�[�Q�b�g��T��

	�_�C�N�X�g���N���X�ɏd�݂̌W����ǉ�����
	�����l��1.0f
	���[�g�v�Z���Ƀm�[�h�ړ��R�X�g�ɁA�ړ���m�[�h�̂��̌W������������Ōv�Z����
	Value	:	float -> mWeightCost
*/



/*
	�t���O(AI)�F
		[Attribute]	:	[Name]			:	[Description]
		Field		:	BallHoldField	:	�t�B�[���h�Ƀ{�[���������Ă��邩
		Field		:	
		Team		:	BallHoldAlly	:	�������{�[���������Ă��邩
		Team		:	BallHoldEnemy	:	�G���{�[���������Ă��邩
		Character	:	BallHoldMe		:	���g�������Ă��邩
		Character	:	InSightEnemy	:	�G�����E�ɂ��邩
		Character	:	InSightAlly		:	���������E�ɂ��邩
		Character	:	ChargedEnergy	:	�G�l���M�[���`���[�W�ς݂�


	Compound Task
		���g���{�[���������Ă���Ƃ�
		���g�ȊO�̖������{�[���������Ă���Ƃ�
		�G���{�[���������Ă���Ƃ�
		�t�B�[���h�Ƀ{�[���������Ă���Ƃ�
	Primitive Task
			�{�[�����E���ɍs��
			�S�[���Ɍ�����
			�G�l���M�[���`���[�W����
			�G�l���M�[���Ȃ��Ȃ����̂ŉB���
			���E�ɂ���G���U������
			�{�[���������Ă��閡����ǂ�������
			�{�[���������Ă��閡�����炠�܂藣�ꂸ�ɍs������
			�����ʂɃ^�[�Q�b�g��T��
			���E��������������ς݂̃^�[�Q�b�g��ǂ�
			�{�[���������Ă���G��ǂ�������
			�G�̃S�[���t�߂�T������
			�����̂̃S�[���t�߂�T������


	//�֐�(AI)
		[Attribute]	:		[Name]			:	[Description]
		Compound	:	BallHoldingMe		:	�������{�[���������Ă���Ƃ�
		Compound	:	BallHoldingAlly		:	�����ȊO�̖������{�[���������Ă���Ƃ�
		Compound	:	BallHoldingEnemy	:	�G���{�[���������Ă���Ƃ�
		Compound	:	BallHoldingField	:	�t�B�[���h�Ƀ{�[���������Ă���Ƃ�
		Primitive	:	MoveToBall			:	�{�[�����E���ɍs��
		Primitive	:	MoveToGoal			:	�S�[���֍s��
		Primitive	:	MoveToHide			:	�`���[�W���邽�߂ɉB���
		Primitive	:	ChargeEnergy		:	�G�l���M�[���`���[�W����
		Primitive	:	InSightAttack		:	���E�ɓ����Ă���G���U������
		Primitive	:	MoveToBallHoldAlly	:	�{�[���������Ă��閡���̋߂��Ɉړ�����
		Primitive	:	SearchEnemyShort	:	�������炠�܂藣�ꂸ�ɓG��T��
		Primitive	:	SerachEnemyAll		:	�����ʂɓG��T��
		Primitive	:	MoveToLookingTarget	:	�����ς݂̓G��ǂ�������
		Primitive	:	MoveToBallTarget	:	�{�[���������Ă���G��ǂ�������
		Primitive	:	SearchForEnemyArea	:	�G�G���A��T������
		Primitive	:	SearchForAllyArea	:	�����G���A��T������



*/

/*
	�[��AI

	Function{
		
	}

	Start{
		//�����{�[���������Ă��邩
		if BallHoldMe == true then

		//�������{�[���������Ă��邩
		elseif BallHoldAlly == true then

		//�G���{�[���������Ă��邩
		elseif BallHoldEnemy == true then

		//�t�B�[���h�Ƀ{�[���������Ă��邩
		elseif BallHoldField == true then
		end

		//�������{�[���������Ă��邩
		if(BallHoldAlly==true){
			
		}
	}


*/
