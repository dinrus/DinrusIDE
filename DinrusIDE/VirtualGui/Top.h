//$ class ТопОкно {
public:
	virtual void   State(int reason);

private:
	TopWindowFrame *frame;
	
	void SyncRect();
	void SyncFrameRect(const Прям& r);
	void DestroyFrame();
	
	friend class Ктрл;

public:
	void GripResize();
//$ };
