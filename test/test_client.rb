require 'libmemcached'
require 'test/unit'

class ClientTest < Test::Unit::TestCase
  include Libmemcached

  def test_raises_config_error
    assert_raises ConfigurationError do
      native_client('abc')
    end
  end

  def test_servers
    client = native_client('--server=127.0.0.1:22122 --socket="/var/run/file.sock"')
    assert_equal ['127.0.0.1:22122', '/var/run/file.sock'], client.servers
  end

  def test_server_by_key
    client = native_client()
    assert_equal '127.0.0.1:11211', client.server_by_key('abc')
  end

  def test_namespace
    client = native_client()
    assert_equal nil, client.namespace
    client.namespace = 'deadbeef'
    assert_equal 'deadbeef', client.namespace
    assert_equal :bad_key_provided, client.namespace('foo'*100)
  end

  def test_invalid_server
    client = native_client('--server=127.0.0.1:99999')
    assert_equal :connection_failure, client.set('foo', 'bar')
  end

  def test_bad_key
    client = native_client()
    assert_equal :bad_key_provided, client.set('foo'*100, 'bar')
  end

  def test_set_and_get
    client = native_client()
    assert_equal true, client.set('foo', 'bar')
    assert_equal ['bar', 0], client.get('foo')
  end

  def test_set_and_get_with_flags
    client = native_client()
    assert_equal true, client.set('foo', 'bar', 0, 123)
    assert_equal ['bar', 123], client.get('foo')
  end

  def test_mget
    values = {
      'foo1' => 'bar1',
      'foo2' => 'bar2',
      'foo3' => 'bar3'
    }

    client = native_client()
    values.each{ |key, val| client.set(key, val) }
    values_with_flags = values.inject(Hash.new){ |h,(k,v)| h[k] = [v, 0]; h }
    assert_equal [:end, values_with_flags], client.mget(values.keys)
  end

  def test_delete
    client = native_client()
    assert_equal true, client.set('foo', 'bar')
    assert_equal ['bar', 0], client.get('foo')
    assert_equal true, client.delete('foo')
    assert_equal false, client.delete('foo')
    assert_equal nil, client.get('foo')
  end

  def native_client(config=nil)
    NativeClient.new(config || '--server=127.0.0.1')
  end
end
